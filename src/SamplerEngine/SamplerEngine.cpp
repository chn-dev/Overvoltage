#include <PluginProcessor.h>
#include <PluginEditor.h>
#include "SamplerEngine.h"

using namespace SamplerEngine;

Engine::Engine( PluginProcessor *pProcessor ) :
   m_pProcessor( pProcessor )
{
   for( size_t i = 0; i < 16; i++ )
   {
      m_Parts.push_back( new Part( i, this ) );
   }
}


Engine::~Engine()
{
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      delete m_Parts[i];
   }
   m_Parts.clear();
}


bool Engine::process( std::vector<OutputBus> &buses, double sampleRate, double bpm )
{
   bool update = false;
   for( Part *pPart : m_Parts )
   {
      update = update || pPart->process( buses, sampleRate, bpm );
   }

   return( update );
}


void Engine::deleteSample( size_t part, Sample *pSample )
{
   m_Parts[part]->deleteSample( pSample );
}


std::list<Sample *> Engine::samples( size_t nPart ) const
{
   return( m_Parts[nPart]->samples() );
}


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


void Engine::noteOn( size_t nPart, int note, int vel )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->noteOn( note, vel );
}


void Engine::noteOff( size_t nPart, int note, int vel )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->noteOff( note, vel );
}


void Engine::pitchbend( size_t nPart, double v )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->setPitchbend( v );
}


void Engine::controllerChange( size_t nPart, int ccNum, double v )
{
   if( nPart >= m_Parts.size() )
      return;

   m_Parts[nPart]->setController( ccNum, v );
}


void Engine::setProcessor( PluginProcessor *pProcessor )
{
   m_pProcessor = pProcessor;
}


Engine *Engine::fromXml( const juce::XmlElement *peOvervoltage )
{
   if( peOvervoltage->getTagName() == "overvoltage" )
   {
      Engine *pEngine = new Engine();

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
      return( pEngine );
   } else
   {
      return( nullptr );
   }
}


juce::XmlElement *Engine::toXml() const
{
   juce::XmlElement *pVt = new juce::XmlElement( "overvoltage" );

   juce::XmlElement *peParts = new juce::XmlElement( "parts" );
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      juce::XmlElement *pePart = m_Parts[i]->toXml();
      peParts->addChildElement( pePart );
   }

   pVt->addChildElement( peParts );

   return( pVt );
}


std::list<Sample *> &Engine::samples( size_t nPart )
{
   return( m_Parts[nPart]->samples() );
}


const std::list<Sample *> &Engine::constSamples( size_t nPart ) const
{
   return( m_Parts[nPart]->samples() );
}


bool Engine::isPlaying( size_t nPart, const Sample *pSample ) const
{
   if( nPart >= m_Parts.size() )
      return( false );
   else
      return( m_Parts[nPart]->isPlaying( pSample ) );
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
