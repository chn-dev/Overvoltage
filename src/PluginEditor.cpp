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

   for( int i = 0; i < 16; i++ )
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
