#include <util.h>
#include "UISectionLFO.h"

using namespace SamplerGUI;

UISectionLFO::UISectionLFO( UIPage *pUIPage, std::string label ) :
   UISection( pUIPage, label )
{
   for( size_t i = 0; i < NUM_LFO; i++ )
   {
      juce::TextButton *pB = new juce::TextButton( stdformat( "{}", i + 1 ) );
      pB->setToggleable( true );
      pB->setClickingTogglesState( true );
      pB->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
      pB->addListener( this );
      pB->setBounds( 2 + 18 * i, 1, 16, 16 );
      addAndMakeVisible( pB );
      m_SelButtons.push_back( pB );
   }

   m_plWaveform = new juce::Label( juce::String(), "Waveform:" );
   m_plWaveform->setBounds( 4, 22, 80, 18 );
   addAndMakeVisible( m_plWaveform );

   m_pcbWaveform = new juce::ComboBox( "Waveform" );
   for( SamplerEngine::LFO::Waveform wf : SamplerEngine::LFO::allWaveforms() )
   {
      m_pcbWaveform->addItem( SamplerEngine::LFO::toString( wf ), wf );
   }
   m_pcbWaveform->addListener( this );
   m_pcbWaveform->setBounds( 84, 22, 120, 18 );
   addAndMakeVisible( m_pcbWaveform );

   m_plFrequency = new juce::Label( juce::String(), "Freq:" );
   m_plFrequency->setBounds( 84 + 120, 22, 40, 18 );
   addAndMakeVisible( m_plFrequency );

   m_psFrequency = new juce::Slider( "Freq" );
   m_psFrequency->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psFrequency->setSliderSnapsToMousePosition( false );
   m_psFrequency->setLookAndFeel( &m_SliderLAF );
   m_psFrequency->addListener( this );
   m_psFrequency->setTextBoxStyle( juce::Slider::TextBoxRight, true, 48, 16 );
   m_psFrequency->setBounds( 84 + 120 + 34, 27, 122, 12 );
   m_psFrequency->setValue( 1.0 );
   addAndMakeVisible( m_psFrequency );

   m_pbSync = new juce::TextButton( "sync" );
   m_pbSync->setToggleable( true );
   m_pbSync->setClickingTogglesState( true );
   m_pbSync->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbSync->addListener( this );
   m_pbSync->setBounds( 84 + 120 + 34 + 122 + 4, 24, 38, 16 );
   addAndMakeVisible( m_pbSync );

   setCurrentLFO( 0 );
}


UISectionLFO::~UISectionLFO()
{
   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      delete m_SelButtons[i];
   }
   m_SelButtons.clear();

   delete m_plWaveform;
   delete m_pcbWaveform;
   delete m_plFrequency;
   delete m_psFrequency;
   delete m_pbSync;
}


void UISectionLFO::sliderValueChanged( Slider *pSlider )
{
   if( m_psFrequency == pSlider )
   {
      double value = m_psFrequency->getValue();
      size_t nLFO = getCurrentLFO();
      bool syncEnabled = m_pbSync->getToggleState();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         if( pSample->getLFO( nLFO )->getSyncEnabled() == syncEnabled )
         {
            if( syncEnabled )
            {
               pSample->getLFO( nLFO )->setSyncBeats( value );

            } else
            {
               pSample->getLFO( nLFO )->setFrequency( value );
            }
         }
      }
   }
}


void UISectionLFO::buttonClicked( Button *pButton )
{
   size_t nLFO = (size_t)-1;
   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      if( m_SelButtons[i] == pButton )
      {
         nLFO = i;
         break;
      }
   }

   if( nLFO != (size_t)-1 )
   {
      setCurrentLFO( nLFO );
   }


   if( pButton == m_pbSync )
   {
      size_t nLFO = getCurrentLFO();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( nLFO );
         pLFO->setSyncEnabled( m_pbSync->getToggleState() );
      }

      m_psFrequency->removeListener( this );
      SamplerEngine::LFO *pLFO = sample()->getLFO( nLFO );
      if( m_pbSync->getToggleState() )
      {
         m_psFrequency->setRange( 0.1, 10.0, 0.1 );
         m_psFrequency->textFromValueFunction = []( double v ) { return( stdformat( "{:.1f}b", v ) ); };
         m_psFrequency->setValue( pLFO->getSyncBeats(), dontSendNotification );
      } else
      {
         m_psFrequency->setRange( 0.01, 10.0, 0.01 );
         m_psFrequency->textFromValueFunction = []( double v ) { return( stdformat( "{:.2f}Hz", v ) ); };
         m_psFrequency->setValue( pLFO->getFrequency(), dontSendNotification );
      }
      m_psFrequency->addListener( this );
   }
}


void UISectionLFO::buttonStateChanged( Button */*pButton*/ )
{
}


void UISectionLFO::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void UISectionLFO::resized()
{
   UISection::resized();
}


void UISectionLFO::samplesUpdated()
{
   const SamplerEngine::Sample *pSample = sample();

   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      m_SelButtons[i]->setVisible( pSample != nullptr );
   }
   m_plWaveform->setVisible( pSample != nullptr );
   m_pcbWaveform->setVisible( pSample != nullptr );
   m_plFrequency->setVisible( pSample != nullptr );
   m_psFrequency->setVisible( pSample != nullptr );
   m_pbSync->setVisible( pSample != nullptr );

   if( pSample )
   {
      m_pcbWaveform->setSelectedId( pSample->getLFO( getCurrentLFO() )->getWaveform(), dontSendNotification );
      SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
      if( pLFO->getSyncEnabled() )
      {
         m_psFrequency->setRange( 0.1, 10.0, 0.1 );
         m_psFrequency->textFromValueFunction = []( double v ) { return( stdformat( "{:.1f}b", v ) ); };
         m_psFrequency->setValue( pLFO->getSyncBeats(), dontSendNotification );
      } else
      {
         m_psFrequency->setRange( 0.01, 10.0, 0.01 );
         m_psFrequency->textFromValueFunction = []( double v ) { return( stdformat( "{:.2f}Hz", v ) ); };
         m_psFrequency->setValue( pLFO->getFrequency(), dontSendNotification );
      }
   }
}


void UISectionLFO::setCurrentLFO( size_t n )
{
   if( n >= m_SelButtons.size() )
      return;

   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      m_SelButtons[i]->setToggleState( i == n, dontSendNotification );
   }

   samplesUpdated();
}


size_t UISectionLFO::getCurrentLFO() const
{
   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      if( m_SelButtons[i]->getToggleState() )
         return( i );
   }

   return( (size_t)-1 );
}


void UISectionLFO::comboBoxChanged( ComboBox *pComboBox )
{
   if( m_pcbWaveform == pComboBox )
   {
      SamplerEngine::LFO::Waveform wf = (SamplerEngine::LFO::Waveform)m_pcbWaveform->getSelectedId();
      size_t nLFO = getCurrentLFO();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         pSample->getLFO( nLFO )->setWaveform( wf );
      }
   }
}
