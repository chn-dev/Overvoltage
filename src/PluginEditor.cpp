#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "util.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor( AudioPluginAudioProcessor& p )
   : AudioProcessorEditor( &p ), processorRef( p )
{
   juce::ignoreUnused (processorRef);
   // Make sure that before the constructor has finished, you've set the
   // editor's size to whatever you need it to be.
   setSize( 1024, 640 );

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

   int yofs = 32;

   m_pKeyboard = new SamplerKeyboard( this );
   addKeyListener( m_pKeyboard );
   m_pKeyboard->setBounds( 0, yofs, 64 + 128, 640 - yofs );
   m_pKeyboard->addListener( &p );
   m_pKeyboard->addSamplerKeyboardListener( &p );
   m_pKeyboard->setWidth( 64 );
   processorRef.juce::MidiKeyboardState::addListener( m_pKeyboard );
   addAndMakeVisible( m_pKeyboard );

   m_pWaveView = new WaveView( this );
   m_pWaveView->setBounds( 64 + 128 + 4, yofs, 1024 - ( 64 + 128 + 4 ), 196 );
   addAndMakeVisible( m_pWaveView );

   m_pSampleSection = new SampleUISection( this );
   m_pSampleSection->setBounds( 64 + 128 + 4, 196 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pSampleSection );

   m_pNameRangesUISection = new NameRangesUISection( this );
   m_pNameRangesUISection->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pNameRangesUISection );

   m_pAEGUISection = new AEGUISection( this );
   m_pAEGUISection->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + 128 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pAEGUISection );

   m_pOutputUISection = new OutputUISection( this );
   m_pOutputUISection->setBounds( 1024 - 64 - 128 - 4, 196 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pOutputUISection );

   for( UISection *pSection : m_UISections )
   {
      pSection->samplesUpdated();
   }

   activatePart( 0 );
}


AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   delete m_pKeyboard;
   delete m_pWaveView;
   delete m_pSampleSection;
   delete m_pNameRangesUISection;
   delete m_pAEGUISection;
   delete m_pOutputUISection;

   for( juce::TextButton *pButton : m_PartButtons )
   {
      delete pButton;
   }

   m_PartButtons.clear();
}


int AudioPluginAudioProcessorEditor::currentPart() const
{
   return( m_CurrentPart );
}


void AudioPluginAudioProcessorEditor::activatePart( int nPart )
{
   if( nPart < 0 || nPart >= m_PartButtons.size() )
      return;

   m_CurrentPart = nPart;
   for( int i = 0; i < m_PartButtons.size(); i++ )
   {
      m_PartButtons[i]->setToggleState( i == nPart, dontSendNotification );
   }
}


void AudioPluginAudioProcessorEditor::buttonClicked( Button *pButton )
{
   auto partIter = std::find( m_PartButtons.begin(), m_PartButtons.end(), pButton );
   if( partIter != m_PartButtons.end() )
   {
      int partIndex = partIter - m_PartButtons.begin();
      activatePart( partIndex );
      m_pKeyboard->clearSelectedSamples();
      repaint();
   }
}


void AudioPluginAudioProcessorEditor::buttonStateChanged( Button *pButton )
{
}


void AudioPluginAudioProcessorEditor::addUISection( UISection *pSection )
{
   if( pSection )
   {
      m_UISections.insert( pSection );
   }
}


void AudioPluginAudioProcessorEditor::removeUISection( UISection *pSection )
{
   if( pSection && ( m_UISections.find( pSection ) != m_UISections.end() ) )
   {
      m_UISections.erase( pSection );
   }
}


AudioPluginAudioProcessor &AudioPluginAudioProcessorEditor::processor() const
{
   return( processorRef );
}


void AudioPluginAudioProcessorEditor::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );
}


void AudioPluginAudioProcessorEditor::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );
}


//==============================================================================
void AudioPluginAudioProcessorEditor::paint( juce::Graphics& g )
{
   juce::ignoreUnused( g );

   g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
   g.fillAll();
}

void AudioPluginAudioProcessorEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
   // subcomponents in your editor..
}


void AudioPluginAudioProcessorEditor::onSampleSelectionUpdated( SamplerKeyboard *pSamplerKeyboard )
{
   for( UISection *pSection : m_UISections )
   {
      pSection->onSampleSelectionUpdated( pSamplerKeyboard );
   }
}
