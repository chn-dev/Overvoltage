#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor( AudioPluginAudioProcessor& p )
   : AudioProcessorEditor( &p ), processorRef( p )
{
   juce::ignoreUnused (processorRef);
   // Make sure that before the constructor has finished, you've set the
   // editor's size to whatever you need it to be.
   setSize( 1024, 640 );

   m_pButton = new juce::TextButton( "Hello!" );
   m_pButton->setBounds( 0, 0, 48, 24 );
   m_pButton->addListener( this );
   addAndMakeVisible( m_pButton );

   m_pKeyboard = new SamplerKeyboard( this );
   addKeyListener( m_pKeyboard );
   m_pKeyboard->setBounds( 0, 0, 64 + 128, 640 );
   m_pKeyboard->addListener( &p );
   m_pKeyboard->addSamplerKeyboardListener( &p );
   m_pKeyboard->setWidth( 64 );
   processorRef.juce::MidiKeyboardState::addListener( m_pKeyboard );
   addAndMakeVisible( m_pKeyboard );

   m_pWaveView = new WaveView( this );
   m_pWaveView->setBounds( 64 + 128 + 4, 0, 1024 - ( 64 + 128 + 4 ), 196 );
   addAndMakeVisible( m_pWaveView );

   m_pSampleSection = new SampleUISection( this );
   m_pSampleSection->setBounds( 64 + 128 + 4, 196 + 4, 192, 128 );
   addAndMakeVisible( m_pSampleSection );

   m_pNameRangesUISection = new NameRangesUISection( this );
   m_pNameRangesUISection->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4, 192, 128 );
   addAndMakeVisible( m_pNameRangesUISection );

   m_pAEGUISection = new AEGUISection( this );
   m_pAEGUISection->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + 128 + 4, 192, 128 );
   addAndMakeVisible( m_pAEGUISection );

   m_pOutputUISection = new OutputUISection( this );
   m_pOutputUISection->setBounds( 1024 - 64 - 128 - 4, 196 + 4, 192, 128 );
   addAndMakeVisible( m_pOutputUISection );

   for( UISection *pSection : m_UISections )
   {
      pSection->samplesUpdated();
   }
}


AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   delete m_pButton;
   delete m_pKeyboard;
   delete m_pWaveView;
   delete m_pSampleSection;
   delete m_pNameRangesUISection;
   delete m_pAEGUISection;
   delete m_pOutputUISection;
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


void AudioPluginAudioProcessorEditor::buttonClicked( juce::Button *pButton )
{
   if( m_pButton == pButton )
   {
      juce::AlertWindow::showMessageBox( juce::AlertWindow::InfoIcon, "Title", "Hello, world!" );
   }
}


//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
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
