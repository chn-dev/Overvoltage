#include "PluginEditor.h"
#include "OutputUISection.h"

OutputUISection::OutputUISection( AudioPluginAudioProcessorEditor *pEditor ) :
   UISection( pEditor, "Output" )
{
   m_pcbOutputBus = new juce::ComboBox( "Output Bus" );
   for( int i = 0; i < editor()->processor().getBusCount( false ); i++ )
   {
      m_pcbOutputBus->addItem( editor()->processor().getBus( false, i )->getName(), i + 1 );
   }
   m_pcbOutputBus->addListener( this );
   addAndMakeVisible( m_pcbOutputBus );

   m_plPan = new juce::Label( juce::String(), "Pan" );
   addAndMakeVisible( m_plPan );

   m_psPan = new juce::Slider( "Pan" );
   m_psPan->setRange( -100, 100, 1 );
   m_psPan->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psPan->setSliderSnapsToMousePosition( false );
   m_psPan->setLookAndFeel( &m_SliderLAF );
   m_psPan->addListener( this );
   m_psPan->setTextValueSuffix( "%" );
   m_psPan->setTextBoxStyle( juce::Slider::TextBoxBelow, true, 48, 28 );
   m_psPan->setColour( juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGBA( 0, 0, 0, 0 ) );
   addAndMakeVisible( m_psPan );

   m_plGain = new juce::Label( juce::String(), "Gain" );
   addAndMakeVisible( m_plGain );

   m_psGain = new juce::Slider( "Gain" );
   m_psGain->setRange( -96, 12, 0.25 );
   m_psGain->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psGain->setSliderSnapsToMousePosition( false );
   m_psGain->setLookAndFeel( &m_SliderLAF );
   m_psGain->addListener( this );
   m_psGain->setTextValueSuffix( "dB" );
   m_psGain->setTextBoxStyle( juce::Slider::TextBoxBelow, true, 64, 28 );
   m_psGain->setColour( juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGBA( 0, 0, 0, 0 ) );
   addAndMakeVisible( m_psGain );
}


OutputUISection::~OutputUISection()
{
   delete m_plPan;
   delete m_psPan;
   delete m_plGain;
   delete m_psGain;
   delete m_pcbOutputBus;
}


void OutputUISection::comboBoxChanged( ComboBox *pComboBox )
{
   if( samples().size() > 0 )
   {
      if( pComboBox == m_pcbOutputBus )
      {
         for( Sample *pSample : samples() )
         {
            pSample->setOutputBus( m_pcbOutputBus->getSelectedId() - 1 );
         }
      }
   }
}


void OutputUISection::sliderValueChanged( Slider *pSlider )
{
   if( samples().size() > 0 )
   {
      if( pSlider == m_psPan )
      {
         for( Sample *pSample : samples() )
         {
            pSample->setPan( m_psPan->getValue() / 100.0 );
         }
      } else
      if( pSlider == m_psGain )
      {
         for( Sample *pSample : samples() )
         {
            pSample->setGain( pow( 10.0, ( ( m_psGain->getValue() ) / 20.0 ) ) );
         }
      }
   }
}


void OutputUISection::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void OutputUISection::resized()
{
   UISection::resized();

   m_psPan->setBounds( 4, 32 - 4 + 32, 180, 25 );
   m_plPan->setBounds( 6, 48 - 4 + 32, 32, 16 );

   m_psGain->setBounds( 4, 64 - 4 + 32, 180, 25 );
   m_plGain->setBounds( 6, 80 - 4 + 32, 32, 16 );

   m_pcbOutputBus->setBounds( 4, 32 - 8, 180, 25 );
}


void OutputUISection::samplesUpdated()
{
   m_plPan->setVisible( sample() != nullptr );
   m_psPan->setVisible( sample() != nullptr );

   m_plGain->setVisible( sample() != nullptr );
   m_psGain->setVisible( sample() != nullptr );

   m_pcbOutputBus->setVisible( sample() != nullptr );

   if( sample() )
   {
      m_psPan->setValue( sample()->getPan() * 100.0 );
      m_psGain->setValue( 20.0 * log10( sample()->getGain() ) );
      m_pcbOutputBus->setSelectedId( sample()->getOutputBus() + 1 );
   }
}


void OutputUISection::labelTextChanged( Label */*pLabel*/ )
{
}

