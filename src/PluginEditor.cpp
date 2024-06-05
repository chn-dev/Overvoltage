#include <iostream>
#include <fstream>

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <SamplerGUI/UIPageZones/UIPageZones.h>

#include "util.h"

//==============================================================================
PluginEditor::PluginEditor( PluginProcessor& p )
   : AudioProcessorEditor( &p ), processorRef( p )
{
   juce::ignoreUnused (processorRef);
   // Make sure that before the constructor has finished, you've set the
   // editor's size to whatever you need it to be.
   setSize( 1024, 640 );

   m_UIPages.push_back( new SamplerGUI::UIPageZones( this ) );
   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      addAndMakeVisible( pPage );
   }

   for( int i = 0; i < SAMPLERENGINE_NUMPARTS; i++ )
   {
      juce::TextButton *pButton = new juce::TextButton( stdformat( "{}", i + 1 ) );
      pButton->setToggleable( true );
      pButton->setClickingTogglesState( true );
      pButton->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
      pButton->addListener( this );
      m_PartButtons.push_back( pButton );
      pButton->setBounds( i * 36 + 8, 7, 32, 18 );
      addAndMakeVisible( pButton );
   }

   m_pExportProgram = new juce::TextButton( "Export prg.." );
   m_pExportProgram->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pExportProgram->addListener( this );
   m_pExportProgram->setBounds( getBounds().getWidth() - 100, 7, 96, 18 );
   addAndMakeVisible( m_pExportProgram );

   m_pImportProgram = new juce::TextButton( "Import prg.." );
   m_pImportProgram->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pImportProgram->addListener( this );
   m_pImportProgram->setBounds( m_pExportProgram->getBounds().getX() - 100, 7, 96, 18 );
   addAndMakeVisible( m_pImportProgram );

   activatePart( 0 );
}


PluginEditor::~PluginEditor()
{
   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      delete pPage;
   }
   m_UIPages.clear();

   for( juce::TextButton *pButton : m_PartButtons )
   {
      delete pButton;
   }

   m_PartButtons.clear();

   delete m_pExportProgram;
   delete m_pImportProgram;
}


SamplerGUI::UIPageZones *PluginEditor::getUIPageZones() const
{
   for( size_t i = 0; i < m_UIPages.size(); i++ )
   {
      SamplerGUI::UIPageZones *pP = dynamic_cast<SamplerGUI::UIPageZones *>( m_UIPages[i] );
      if( pP )
      {
         return( pP );
      }
   }

   return( nullptr );
}


std::set<SamplerEngine::Sample *> PluginEditor::getSelectedSamples() const
{
   return( getUIPageZones()->getSamplerKeyboard()->selectedSamples() );
}


bool PluginEditor::isSoloEnabled() const
{
   return( getUIPageZones()->isSoloEnabled() );
}


size_t PluginEditor::currentPart() const
{
   return( m_CurrentPart );
}


void PluginEditor::activatePart( size_t nPart )
{
   if( nPart >= m_PartButtons.size() )
      return;

   m_CurrentPart = nPart;
   for( size_t i = 0; i < m_PartButtons.size(); i++ )
   {
      m_PartButtons[i]->setToggleState( i == nPart, dontSendNotification );
   }

   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      pPage->currentPartChanged( nPart );
   }
}


void PluginEditor::buttonClicked( Button *pButton )
{
   auto partIter = std::find( m_PartButtons.begin(), m_PartButtons.end(), pButton );
   if( partIter != m_PartButtons.end() )
   {
      size_t partIndex = (size_t)( partIter - m_PartButtons.begin() );
      activatePart( partIndex );
      repaint();
   } else
   if( pButton == m_pImportProgram )
   {
      juce::FileChooser ch = juce::FileChooser( "Import program..", juce::File(), "*.xml", false, true, this );
      if( ch.browseForFileToOpen( nullptr ) )
      {
         std::string fname = ch.getResult().getFullPathName().toStdString();
         std::ifstream file;
         file.open( fname, std::ifstream::in );
         if( file.is_open() )
         {
            file.seekg( 0, std::ios::end );
            size_t fsize = file.tellg();
            file.seekg( 0, std::ios::beg );
            char *pTmp = new char[fsize];
            file.read( pTmp, fsize );
            std::string xml = std::string( pTmp );
            delete pTmp;
            file.close();

            std::unique_ptr<juce::XmlElement> pXml = parseXML( xml );
            if( pXml.get() )
            {
               processor().samplerEngine()->importPart( currentPart(), pXml.get() );
               repaint();
            }
         }
      }
   } else
   if( pButton == m_pExportProgram )
   {
      juce::FileChooser ch = juce::FileChooser( "Export current program..", juce::File(), "*.xml", false, true, this );
      if( ch.browseForFileToSave( true ) )
      {
         std::string fname = ch.getResult().getFullPathName().toStdString();
         juce::XmlElement *pPartXml = processor().samplerEngine()->getPart( currentPart() )->toXml();
         std::string partXml = pPartXml->toString().toStdString();
         delete pPartXml;

         std::ofstream file;
         file.open( fname, std::ofstream::out );
         if( file.is_open() )
         {
            file.write( partXml.c_str(), partXml.size() );
            file.close();
         }
      }
   }
}


void PluginEditor::buttonStateChanged( Button */*pButton*/ )
{
}


PluginProcessor &PluginEditor::processor() const
{
   return( processorRef );
}


void PluginEditor::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );
}


void PluginEditor::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );
}


//==============================================================================
void PluginEditor::paint( juce::Graphics& g )
{
   juce::ignoreUnused( g );

   g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
   g.fillAll();
}

void PluginEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
   // subcomponents in your editor..
}


void PluginEditor::onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pSamplerKeyboard )
{
   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      pPage->onSampleSelectionUpdated( pSamplerKeyboard );
   }
}
