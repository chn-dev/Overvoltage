#include "SamplerEngine.h"

using namespace Overvoltage;

SamplerEngine::SamplerEngine()
{
   for( size_t i = 0; i < 16; i++ )
   {
      m_Parts.push_back( new Part( i ) );
   }
}


SamplerEngine::~SamplerEngine()
{
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      delete m_Parts[i];
   }
   m_Parts.clear();
}


bool SamplerEngine::process( std::vector<OutputBus> &buses, double sampleRate )
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

         if( !pVoice->process( pLeft, pRight, buses[busNum].getNumSamples(), sampleRate ) )
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


std::list<Sample *> SamplerEngine::getSamplesByMidiNoteAndVelocity( size_t part, int note, int vel ) const
{
   std::list<Sample *> result;

   for( Sample *pSample : m_Parts[part]->samples() )
   {
      if( pSample->matchesMidiNote( note ) && pSample->matchesVelocity( vel ) )
      {
         result.push_back( pSample );
      }
   }

   return( result );
}


void SamplerEngine::deleteSample( size_t part, Sample *pSample )
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

   m_Parts[part]->deleteSample( pSample );
}


void SamplerEngine::stopVoice( const Voice *pVoice )
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


std::list<Sample *> SamplerEngine::samples( size_t nPart ) const
{
   return( m_Parts[nPart]->samples() );
}


void SamplerEngine::noteOn( size_t nPart, int note, int vel )
{
   std::list<Sample *> s = getSamplesByMidiNoteAndVelocity( nPart, note, vel );
   for( Sample *pSample : s )
   {
      Voice *pVoice = new Voice( pSample, note, vel );
      m_Voices.insert( std::pair{ note, pVoice } );
   }
}


void SamplerEngine::noteOff( size_t nPart, int note, int /*vel*/ )
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


SamplerEngine *SamplerEngine::fromXml( const juce::XmlElement *peOvervoltage )
{
   if( peOvervoltage->getTagName() == "overvoltage" )
   {
      SamplerEngine *pEngine = new SamplerEngine();

      for( int i = 0; peOvervoltage->getChildElement( i ); i++ )
      {
         if( peOvervoltage->getChildElement( i )->getTagName() == "parts" )
         {
            juce::XmlElement *peParts = peOvervoltage->getChildElement( i );
            for( int nPart = 0; peParts->getChildElement( nPart ); nPart++ )
            {
               if( peParts->getChildElement( nPart )->getTagName() == "part" )
               {
                  juce::XmlElement *pePart = peParts->getChildElement( nPart );
                  Part *pPart = Part::fromXml( pePart );
                  if( pPart )
                  {
                     if( pEngine->m_Parts[pPart->getPartNum()] )
                     {
                        delete pEngine->m_Parts[pPart->getPartNum()];
                     }
                     pEngine->m_Parts[pPart->getPartNum()] = pPart;
                  }
               }
            }
         }
      }
      return( pEngine );
   } else
   {
      return( nullptr );
   }
}


juce::XmlElement *SamplerEngine::toXml() const
{
   juce::XmlElement *pVt = new juce::XmlElement( "overvoltage" );

   juce::XmlElement *peParts = new juce::XmlElement( "parts" );
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      juce::XmlElement *pePart = m_Parts[i]->getStateInformation();
      peParts->addChildElement( pePart );
   }

   pVt->addChildElement( peParts );

   return( pVt );
}


std::list<Sample *> &SamplerEngine::samples( size_t nPart )
{
   return( m_Parts[nPart]->samples() );
}


const std::list<Sample *> &SamplerEngine::constSamples( size_t nPart ) const
{
   return( m_Parts[nPart]->samples() );
}


std::set<int> SamplerEngine::allPlayingMidiNotes() const
{
   std::set<int> result;
   for( auto i = m_Voices.begin(); i != m_Voices.end(); i++ )
   {
      result.insert( i->first );
   }

   return( result );
}


bool SamplerEngine::isPlaying( const Sample *pSample ) const
{
   std::set<int> midiNotes = allPlayingMidiNotes();
   for( auto v : m_Voices )
   {
      if( v.second->sample() == pSample )
      {
         return( true );
      }
   }

   return( false );
}


OutputBus::OutputBus() :
   m_Valid( false ),
   m_NumSamples( ~(decltype( m_NumSamples ))0 ),
   m_WritePointers( std::vector<float *>() )
{
}


OutputBus::OutputBus( size_t numSamples, std::vector<float *> writePointers ) :
   m_Valid( true ),
   m_NumSamples( numSamples ),
   m_WritePointers( writePointers )
{
}


OutputBus::~OutputBus()
{
}


size_t OutputBus::getNumSamples() const
{
   return( m_NumSamples );
}


std::vector<float *> &OutputBus::getWritePointers()
{
   return( m_WritePointers );
}


bool OutputBus::isValid() const
{
   return( m_Valid );
}
