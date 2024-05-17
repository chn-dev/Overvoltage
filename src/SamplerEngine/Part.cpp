#include <string>

#include "SamplerEngine.h"
#include "util.h"

using namespace SamplerEngine;

Part::Part( size_t partNum, Engine *pEngine ) :
   m_PartNum( partNum ),
   m_pEngine( pEngine ),
   m_Pitchbend( 0.0 )
{
}


Part::~Part()
{
}


bool Part::process( std::vector<OutputBus> &buses, double sampleRate, double bpm )
{
   std::set<Voice *> stoppedVoices;
   for( auto k = m_Voices.begin(); k != m_Voices.end(); k++ )
   {
      Voice *pVoice = k->second;
      size_t busNum;
      if( pVoice->sample()->getOutputBus() < 0 )
         busNum = 0;
      else
         busNum = (size_t)pVoice->sample()->getOutputBus();

      if( busNum >= buses.size() )
      {
         stoppedVoices.insert( pVoice );
      } else
         if( !buses[busNum].isValid() )
         {
            stoppedVoices.insert( pVoice );
         } else
            if( buses[busNum].getWritePointers().size() != 2 )
            {
               stoppedVoices.insert( pVoice );
            } else
            {
               float *pLeft = buses[busNum].getWritePointers()[0];
               float *pRight = buses[busNum].getWritePointers()[1];

               if( !pVoice->process( pLeft, pRight, buses[busNum].getNumSamples(), sampleRate, bpm ) )
               {
                  stoppedVoices.insert( pVoice );
               }
            }
   }

   if( stoppedVoices.size() > 0 )
   {
      for( auto v : stoppedVoices )
      {
         stopVoice( v );
      }
      return( true );
   } else
   {
      return( false );
   }
}


void Part::stopVoice( const Voice *pVoice )
{
   for( auto v = m_Voices.begin(); v != m_Voices.end(); v++ )
   {
      if( v->second == pVoice )
      {
         m_Voices.erase( v );
         delete pVoice;
         return;
      }
   }
}


juce::XmlElement *Part::toXml() const
{
   juce::XmlElement *pePart = new juce::XmlElement( "part" );
   pePart->setAttribute( "num", stdformat( "{}", m_PartNum ) );

   juce::XmlElement *peSamples = new juce::XmlElement( "samples" );
   for( Sample *pSample : m_Samples )
   {
      juce::XmlElement *peSample = pSample->toXml();
      peSamples->addChildElement( peSample );

   }
   pePart->addChildElement( peSamples );

   return( pePart );
}


Part *Part::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "part" )
      return( nullptr );

   size_t partNum = std::stoul( pe->getStringAttribute( "num" ).toStdString() );

   Part *pPart = new Part( partNum );

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *peChild = pe->getChildElement( i );
      if( peChild->getTagName() == "samples" )
      {
         for( int nSample = 0; peChild->getChildElement( nSample ); nSample++ )
         {
            if( peChild->getChildElement( nSample )->getTagName() == "sample" )
            {
               juce::XmlElement *peSample = peChild->getChildElement( nSample );
               Sample *pSample = Sample::fromXml( peSample );
               if( pSample )
               {
                  pPart->m_Samples.push_back( pSample );
               }
            }
         }
      }
   }

   return( pPart );
}


size_t Part::getPartNum() const
{
   return( m_PartNum );
}


std::list<Sample *> &Part::samples()
{
   return( m_Samples );
}


const std::list<Sample *> &Part::constSamples() const
{
   return( m_Samples );
}


void Part::addSample( Sample *pSample )
{
   if( containsSample( pSample ) )
      return;

   m_Samples.push_back( pSample );
}


void Part::removeSample( Sample *pSample )
{
   std::vector<Voice *> voicesToStop;
   for( std::pair<int, Voice *> sv : m_Voices )
   {
      if( sv.second->sample() == pSample )
      {
         voicesToStop.push_back( sv.second );
      }
   }

   for( Voice *pVoice : voicesToStop )
   {
      stopVoice( pVoice );
   }

   if( containsSample( pSample ) )
   {
      m_Samples.remove( pSample );
   }
}


void Part::deleteSample( Sample *pSample )
{
   std::vector<Voice *> voicesToStop;
   for( std::pair<int, Voice *> sv : m_Voices )
   {
      if( sv.second->sample() == pSample )
      {
         voicesToStop.push_back( sv.second );
      }
   }

   for( Voice *pVoice : voicesToStop )
   {
      stopVoice( pVoice );
   }

   if( std::find( m_Samples.begin(), m_Samples.end(), pSample ) != m_Samples.end() )
   {
      m_Samples.remove( pSample );
      delete pSample;
   }
}


std::list<Sample *> Part::getSamplesByMidiNoteAndVelocity( int note, int vel ) const
{
   std::list<Sample *> result;

   for( Sample *pSample : m_Samples )
   {
      if( pSample->matchesMidiNote( note ) && pSample->matchesVelocity( vel ) )
      {
         result.push_back( pSample );
      }
   }

   return( result );
}


bool Part::isPlaying( const Sample *pSample ) const
{
   for( auto v : m_Voices )
   {
      if( v.second->sample() == pSample )
      {
         return( true );
      }
   }

   return( false );
}


void Part::noteOn( int note, int vel )
{
   std::list<Sample *> s = getSamplesByMidiNoteAndVelocity( note, vel );

   bool isSoloEnabled = false;
   std::set<Sample *> selectedSamples;
   if( m_pEngine )
   {
      isSoloEnabled = m_pEngine->isSoloEnabled();
      if( isSoloEnabled )
      {
         selectedSamples = m_pEngine->getSelectedSamples();
      }
   }

   for( Sample *pSample : s )
   {
      if( !isSoloEnabled || ( selectedSamples.find( pSample ) != selectedSamples.end() ) )
      {
         Voice *pVoice = new Voice( this, pSample, note, vel );
         m_Voices.insert( std::pair{ note, pVoice } );
      }
   }
}


void Part::noteOff( int note, int /*vel*/ )
{
   if( m_Voices.count( note ) > 0 )
   {
      auto v = m_Voices.equal_range( note );
      for( auto voice = v.first; voice != v.second; voice++ )
      {
         Voice *pVoice = voice->second;
         pVoice->noteOff();
      }
   }
}


void Part::setPitchbend( double v )
{
   m_Pitchbend = v;
}


double Part::getPitchbend() const
{
   return( m_Pitchbend );
}


void Part::setController( int ccNum, double v )
{
   m_ControllerValues[ccNum] = v;
}


double Part::getController( int ccNum ) const
{
   if( m_ControllerValues.find( ccNum ) == m_ControllerValues.end() )
   {
      return( 0.0 );
   } else
   {
      return( m_ControllerValues.at( ccNum ) );
   }
}


void Part::setEngine( Engine *pEngine )
{
   m_pEngine = pEngine;
}


bool Part::containsSample( const Sample *pSample ) const
{
   return( std::find( m_Samples.begin(), m_Samples.end(), pSample ) != m_Samples.end() );
}
