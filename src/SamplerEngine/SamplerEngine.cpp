/*----------------------------------------------------------------------------*/
/*!
\file SamplerEngine.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the sampler engine
*/
/*----------------------------------------------------------------------------*/
#include <PluginProcessor.h>
#include <PluginEditor.h>
#include "SamplerEngine.h"

using namespace SamplerEngine;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
Engine::Engine( PluginProcessor *pProcessor ) :
   m_pProcessor( pProcessor )
{
   for( size_t i = 0; i < SAMPLERENGINE_NUMPARTS; i++ )
   {
      m_Parts.push_back( new Part( i, this ) );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
Engine::~Engine()
{
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      delete m_Parts[i];
   }
   m_Parts.clear();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Process the engine.
\param buses A vector of all output buses
\param sampleRate The sample rate in Hz
\param bpm The host's tempo in bpm
\return true If there has been an update
*/
/*----------------------------------------------------------------------------*/
bool Engine::process( std::vector<OutputBus> &buses, double sampleRate, double bpm )
{
   bool update = false;
   for( Part *pPart : m_Parts )
   {
      update = update || pPart->process( buses, sampleRate, bpm );
   }

   return( update );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Delete a specific sample from a specific part
\param part The part number (0..15)
\param pSample The sample
*/
/*----------------------------------------------------------------------------*/
void Engine::deleteSample( size_t part, Sample *pSample )
{
   m_Parts[part]->deleteSample( pSample );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Retrieve all samples from a specific part
\param nPart The part number (0..15)
\return A list of all samples of the part
*/
/*----------------------------------------------------------------------------*/
std::list<Sample *> Engine::samples( size_t nPart ) const
{
   return( m_Parts[nPart]->samples() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the solo function is enabled
*/
/*----------------------------------------------------------------------------*/
bool Engine::isSoloEnabled() const
{
   bool bSolo = false;

   if( m_pProcessor )
   {
      if( m_pProcessor->pluginEditor() )
      {
         bSolo = m_pProcessor->pluginEditor()->isSoloEnabled();
      }
   }

   return( bSolo );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A set of all samples selected by the user
*/
/*----------------------------------------------------------------------------*/
std::set<SamplerEngine::Sample *> Engine::getSelectedSamples() const
{
   std::set<SamplerEngine::Sample *> selSamples;

   if( m_pProcessor )
   {
      if( m_pProcessor->pluginEditor() )
      {
         selSamples = m_pProcessor->pluginEditor()->getSelectedSamples();
      }
   }

   return( selSamples );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Trigger note on.
\param nPart The part number (0..15)
\param note The MIDI note number (0..255)
\param vel The velocity (0..127)
*/
/*----------------------------------------------------------------------------*/
void Engine::noteOn( size_t nPart, int note, int vel )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->noteOn( note, vel );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Trigger note off.
\param nPart The part number (0..15)
\param note The MIDI note number (0..255)
\param vel The velocity (0..127)
*/
/*----------------------------------------------------------------------------*/
void Engine::noteOff( size_t nPart, int note, int vel )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->noteOff( note, vel );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Set the pitchbend value of a specific part.
\param nPart The part number (0..15)
\param v The pitchbend value (-1..1)
*/
/*----------------------------------------------------------------------------*/
void Engine::pitchbend( size_t nPart, double v )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->setPitchbend( v );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Set a MIDI controller value
\param nPart The part number (0..15)
\param ccNum The controller number (0..255)
\param v The value
*/
/*----------------------------------------------------------------------------*/
void Engine::controllerChange( size_t nPart, int ccNum, double v )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->setController( ccNum, v );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pProcessor
*/
/*----------------------------------------------------------------------------*/
void Engine::setProcessor( PluginProcessor *pProcessor )
{
   m_pProcessor = pProcessor;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Import a part from XML data.
\param nPart The part number
\param pXmlPart The XML data
*/
/*----------------------------------------------------------------------------*/
void Engine::importPart( size_t nPart, xmlNode *pXmlPart )
{
   if( nPart >= SAMPLERENGINE_NUMPARTS )
      return;

   Part *pPart = Part::fromXml( pXmlPart );
   if( pPart )
   {
      pPart->setPartNum( nPart );
      delete m_Parts[nPart];
      m_Parts[nPart] = pPart;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Reconstruct a sample engine object from a previously generated XML element (see toXml()).
\param pe The XML element
\return Pointer to the Engine object or nullptr on error
*/
/*----------------------------------------------------------------------------*/
Engine *Engine::fromXml( xmlNode *peOvervoltage )
{
   if( std::string( (char*)peOvervoltage->name ) == "overvoltage" )
   {
      Engine *pEngine = new Engine();
      for( xmlNode *pNode = peOvervoltage->children; pNode; pNode = pNode->next )
      {
         if( pNode->type == XML_ELEMENT_NODE )
         {
            if( std::string( (char*)pNode->name ) == "parts" )
            {
               for( xmlNode *peParts = pNode->children; peParts; peParts = peParts->next )
               {
                  if( peParts->type == XML_ELEMENT_NODE )
                  {
                     if( std::string( (char*)peParts->name ) == "part" )
                     {
                        Part *pPart = Part::fromXml( peParts );
                        if( pPart )
                        {
                           pPart->setEngine( pEngine );
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
         }
      }
      return( pEngine );
   } else
   {
      return( nullptr );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Create an XML element from the Engine settings.
\return Pointer to the new XML element
*/
/*----------------------------------------------------------------------------*/
xmlNode *Engine::toXml() const
{
   xmlNode *pVt = xmlNewNode( nullptr, (xmlChar *)"overvoltage" );

   xmlNode *peParts = xmlNewNode( nullptr, (xmlChar *)"parts" );
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      xmlNode *pePart = m_Parts[i]->toXml();
      xmlAddChild( peParts, pePart );
   }

   xmlAddChild( pVt, peParts );

   return( pVt );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param nPart The part number (0..15)
\return A list of all samples of the specified part
*/
/*----------------------------------------------------------------------------*/
std::list<Sample *> &Engine::samples( size_t nPart )
{
   return( m_Parts[nPart]->samples() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param nPart The part number (0..15)
\return A list of all samples of the specified part
*/
/*----------------------------------------------------------------------------*/
const std::list<Sample *> &Engine::constSamples( size_t nPart ) const
{
   return( m_Parts[nPart]->samples() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param nPart The part number (0..15)
\param pSample A sample
\return true if the specified part is playing the specified sample
*/
/*----------------------------------------------------------------------------*/
bool Engine::isPlaying( size_t nPart, const Sample *pSample ) const
{
   if( nPart >= m_Parts.size() )
      return( false );
   else
      return( m_Parts[nPart]->isPlaying( pSample ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param nPart The part number (0..15)
\return The part
*/
/*----------------------------------------------------------------------------*/
Part *Engine::getPart( size_t nPart )
{
   if( nPart >= m_Parts.size() )
      return( nullptr );
   else
      return( m_Parts[nPart] );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample A sample
\return The part containing the specified sample or nullptr if not found
*/
/*----------------------------------------------------------------------------*/
Part *Engine::findPart( const Sample *pSample )
{
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      if( m_Parts[i]->containsSample( pSample ) )
         return( m_Parts[i] );
   }

   return( nullptr );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Contructor
*/
/*----------------------------------------------------------------------------*/
OutputBus::OutputBus() :
   m_Valid( false ),
   m_NumSamples( ~(decltype( m_NumSamples ))0 ),
   m_WritePointers( std::vector<float *>() )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
OutputBus::OutputBus( size_t numSamples, std::vector<float *> writePointers ) :
   m_Valid( true ),
   m_NumSamples( numSamples ),
   m_WritePointers( writePointers )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
OutputBus::~OutputBus()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of samples within the output bus
*/
/*----------------------------------------------------------------------------*/
size_t OutputBus::getNumSamples() const
{
   return( m_NumSamples );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The write pointers for the output bus (there should be 2)
*/
/*----------------------------------------------------------------------------*/
std::vector<float *> &OutputBus::getWritePointers()
{
   return( m_WritePointers );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the output bus is valid
*/
/*----------------------------------------------------------------------------*/
bool OutputBus::isValid() const
{
   return( m_Valid );
}

