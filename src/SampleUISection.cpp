#include "PluginEditor.h"
#include "SampleUISection.h"

SampleUISection::SampleUISection( AudioPluginAudioProcessorEditor *pEditor ) :
   UISection( pEditor, "Sample" )
{
   m_plDetune = new juce::Label( "Detune", "Detune:" );
   addAndMakeVisible( m_plDetune );

   m_psDetune = new juce::Slider( "Detune" );
   m_psDetune->setRange( - 100.0, 100.0, 1.0 );
   m_psDetune->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psDetune->setTextValueSuffix( "cts" );
   m_psDetune->setTextBoxStyle( juce::Slider::TextBoxBelow, true, 48, 14 );
   m_psDetune->setSliderSnapsToMousePosition( false );
   m_psDetune->setLookAndFeel( &m_SliderLAF );
   m_psDetune->addListener( this );
   m_psDetune->setColour( juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGBA( 0, 0, 0, 0 ) );
   addAndMakeVisible( m_psDetune );

   m_pbReverse = new juce::TextButton( "Reverse" );
   m_pbReverse->setToggleable( true );
   m_pbReverse->setClickingTogglesState( true );
   m_pbReverse->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbReverse->addListener( this );
   addAndMakeVisible( m_pbReverse );

   m_pcbPlayMode = new juce::ComboBox( "Play Mode" );
   for( Sample::PlayMode pm : Sample::allPlayModes() )
   {
      m_pcbPlayMode->addItem( Sample::toString( pm ), pm );
   }
   m_pcbPlayMode->addListener( this );
   addAndMakeVisible( m_pcbPlayMode );
}


SampleUISection::~SampleUISection()
{
   delete m_plDetune;
   delete m_psDetune;
   delete m_pbReverse;
   delete m_pcbPlayMode;
}


void SampleUISection::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void SampleUISection::resized()
{
   UISection::resized();

   m_plDetune->setBounds( 0, 32, 64, 12 );
   m_psDetune->setBounds( 48, 32, 150, 25 );
   m_pbReverse->setBounds( 4, 76, 56, 20 );
   m_pcbPlayMode->setBounds( 68, 76, 120, 20 );
}


void SampleUISection::buttonClicked( Button *pButton )
{
   if( pButton == m_pbReverse )
   {
      for( Sample *pSample : samples() )
      {
         pSample->setReverse( m_pbReverse->getToggleState() );
      }
   }
}


void SampleUISection::buttonStateChanged( Button */*pButton*/ )
{
}


void SampleUISection::comboBoxChanged( ComboBox *pComboBox )
{
   if( m_pcbPlayMode == pComboBox )
   {
      for( Sample *pSample : samples() )
      {
         pSample->setPlayMode( (Sample::PlayMode)m_pcbPlayMode->getSelectedId() );
      }
   }
}

void SampleUISection::samplesUpdated()
{
   const Sample *pSample = sample();

   m_plDetune->setVisible( pSample != nullptr );
   m_psDetune->setVisible( pSample != nullptr );
   m_pbReverse->setVisible( pSample != nullptr );
   m_pcbPlayMode->setVisible( pSample != nullptr );

   if( pSample )
   {
      m_psDetune->setValue( pSample->getDetune() );
      m_pbReverse->setToggleState( pSample->getReverse(), dontSendNotification );
      m_pcbPlayMode->setSelectedId( pSample->getPlayMode() );
   }
}


void SampleUISection::sliderValueChanged( Slider *pSlider )
{
   if( ( m_psDetune == pSlider ) && ( samples().size() > 0 ) )
   {
      for( Sample *pSample : samples() )
      {
         pSample->setDetune( (float)pSlider->getValue() );
      }
   }
}
