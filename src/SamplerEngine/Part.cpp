/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of chn's Overvoltage.                                   *
 *                                                                             *
 *  Overvoltage is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *          
 *  Overvoltage is distributed in the hope that it will be useful, but         * 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Overvoltage. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file Part.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a MIDI Part of the sampler engine
*/
/*----------------------------------------------------------------------------*/
#include <algorithm>
#include <string>

#include "SamplerEngine.h"
#include "util.h"

using namespace SamplerEngine;

/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
Part::Part( size_t partNum, Engine *pEngine ) :
   m_PartNum( partNum ),
   m_pEngine( pEngine ),
   m_Pitchbend( 0.0 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
Part::~Part()
{
   stopAllVoices();

   for( Sample *pSample : m_Samples )
   {
      delete pSample;
   }
   m_Samples.clear();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called on a regular bases to process the part and produce new audio data.
\param buses The output buses to store new audio data in
\param sampleRate The sample rate in Hz
\param bpm The host's tempo in bom
\return true if voices have been stopped
*/
/*----------------------------------------------------------------------------*/
bool Part::process( std::vector<OutputBus> &buses, double sampleRate, double bpm )
{
   std::set<Voice *> stoppedVoices;
   int n = 0;
   for( auto k = m_Voices.begin(); k != m_Voices.end(); k++ )
   {
      n++;
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Stop all voices.
*/
/*----------------------------------------------------------------------------*/
void Part::stopAllVoices()
{
   for( auto v : m_Voices )
   {
      delete v.second;
   }
   m_Voices.clear();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Stop a specific voice.
\param pVoice The voice to be stopped
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Create an XML element from the Part settings.
\return Pointer to the new XML element
*/
/*----------------------------------------------------------------------------*/
xmlNode *Part::toXml() const
{
   xmlNode *pePart = xmlNewNode( nullptr, (xmlChar *)"part" );
   xmlNewProp( pePart, (xmlChar *)"num", (xmlChar *)stdformat( "{}", m_PartNum ).c_str() );

   xmlNode *peSamples = xmlNewNode( nullptr, (xmlChar *)"samples" );
   for( Sample *pSample : m_Samples )
   {
      xmlNode *peSample = pSample->toXml();
      xmlAddChild( peSamples, peSample );

   }
   xmlAddChild( pePart, peSamples );

   return( pePart );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Reconstruct a Part object from a previously generated XML element (see toXml()).
\param pe The XML element
\return Pointer to the Part object or nullptr on error
*/
/*----------------------------------------------------------------------------*/
Part *Part::fromXml( xmlNode *pe )
{
   if( std::string( (char*)pe->name ) != "part" )
      return( nullptr );

   size_t partNum = 0;

   for( xmlAttr *pAttr = pe->properties; pAttr; pAttr = pAttr->next )
   {
      if( pAttr->type == XML_ATTRIBUTE_NODE )
      {
         std::string name = std::string( (char*)pAttr->name );
         xmlChar* pValue = xmlNodeListGetString( pe->doc, pAttr->children, 1 );
         std::string value = std::string( (char*)pValue );
         xmlFree( pValue );

         if( name == "num" )
         {
            partNum = std::stoul( value );
         }
      }
   }

   Part *pPart = new Part( partNum );

   for( xmlNode *p = pe->children; p; p = p->next )
   {
      if( p->type == XML_ELEMENT_NODE )
      {
         if( std::string( (char*)p->name ) == "samples" )
         {
            for( xmlNode *pSamples = p->children; pSamples; pSamples = pSamples->next )
            {
               if( pSamples->type == XML_ELEMENT_NODE )
               {
                  if( std::string( (char*)pSamples->name ) == "sample" )
                  {
                     Sample *pSample = Sample::fromXml( pSamples );
                     if( pSample )
                     {
                        pPart->m_Samples.push_back( pSample );
                     }
                  }
               }
            }
         }
      }
   }

   return( pPart );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return This part's number (0..15)
*/
/*----------------------------------------------------------------------------*/
size_t Part::getPartNum() const
{
   return( m_PartNum );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void Part::setPartNum( size_t n )
{
   m_PartNum = n;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A list of all samples within this part
*/
/*----------------------------------------------------------------------------*/
std::list<Sample *> &Part::samples()
{
   return( m_Samples );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A list of all samples within this part
*/
/*----------------------------------------------------------------------------*/
const std::list<Sample *> &Part::constSamples() const
{
   return( m_Samples );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample A sample to be added to this part
*/
/*----------------------------------------------------------------------------*/
void Part::addSample( Sample *pSample )
{
   if( containsSample( pSample ) )
      return;

   m_Samples.push_back( pSample );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample The sample to be removed from this part. All currently playing
voices containing the sample will be stopped. The sample itself will not  be
deleted from memory.
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample The sample to be removed from this part. All currently playing
voices containing the sample will be stopped. The sample itself will  be
deleted from memory.
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Retrieve all samples matching a specific MIDI note number and velocity.
\param note The MIDI note number
\param vel the MIDI velocity
\return A list of all matching samples

*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample The sample
\return true if the specified sample is currently being played
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Trigger a note on event.
\param note The MIDI note number
\param vel The MIDI velocity
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Trigger a note off event.
\param note The MIDI note number
\param vel The MIDI velocity
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param v The pitchbend value (-1..0)
*/
/*----------------------------------------------------------------------------*/
void Part::setPitchbend( double v )
{
   m_Pitchbend = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The current pitchbend value (-1..0)
*/
/*----------------------------------------------------------------------------*/
double Part::getPitchbend() const
{
   return( m_Pitchbend );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Set controller value.
\param ccNum The controller's number (0..255)
\param v The controller's value (0..1)
*/
/*----------------------------------------------------------------------------*/
void Part::setController( int ccNum, double v )
{
   m_ControllerValues[ccNum] = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Get controller value
\param ccNum The controller's number (0..255)
\return The controller's value
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pEngine The sampler engine
*/
/*----------------------------------------------------------------------------*/
void Part::setEngine( Engine *pEngine )
{
   m_pEngine = pEngine;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample A sample
\return true if this part contains the given sample
*/
/*----------------------------------------------------------------------------*/
bool Part::containsSample( const Sample *pSample ) const
{
   return( std::find( m_Samples.begin(), m_Samples.end(), pSample ) != m_Samples.end() );
}

