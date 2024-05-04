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

   m_pcbWaveform = new juce::ComboBox( "Waveform" );
   for( SamplerEngine::LFO::Waveform wf : SamplerEngine::LFO::allWaveforms() )
   {
      m_pcbWaveform->addItem( SamplerEngine::LFO::toString( wf ), wf );
   }
   m_pcbWaveform->addListener( this );
   m_pcbWaveform->setBounds( 4, 22, 100, 18 );
   addAndMakeVisible( m_pcbWaveform );

   m_plRate = new juce::Label( juce::String(), "Rate:" );
   m_plRate->setBounds( 4 + 98, 21, 40, 18 );
   addAndMakeVisible( m_plRate );

   m_pcRate = new CycleComponent();
   m_pcRate->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcRate->addListener( this );
   m_pcRate->setJustificationType( juce::Justification::centred );
   m_pcRate->setBounds( 4 + 100 + 34, 24, 48, 13 );
   addAndMakeVisible( m_pcRate );

   m_pbSync = new juce::TextButton( "sync" );
   m_pbSync->setToggleable( true );
   m_pbSync->setClickingTogglesState( true );
   m_pbSync->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbSync->addListener( this );
   m_pbSync->setBounds( 4 + 100 + 34 + 48 + 2, 23, 38, 16 );
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

   delete m_pcbWaveform;
   delete m_plRate;
   delete m_pcRate;
   delete m_pbSync;
}


void UISectionLFO::labelTextChanged( Label *pLabel )
{
   if( m_pcRate == pLabel )
   {
      double value = m_pcRate->getCurrentItemValue();
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


void UISectionLFO::sliderValueChanged( Slider *pSlider )
{
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
      nLFO = getCurrentLFO();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( nLFO );
         pLFO->setSyncEnabled( m_pbSync->getToggleState() );
      }

      SamplerEngine::LFO *pLFO = sample()->getLFO( nLFO );
      if( pLFO->getSyncEnabled() )
      {
         m_pcRate->setItems( 0.1, 10.0, 0.1, "{:.1f}", "b" );
         m_pcRate->setCurrentItemByValue( pLFO->getFrequency() );
      } else
      {
         m_pcRate->setItems( 0.01, 10.0, 0.01, "{:.2f}", "Hz" );
         m_pcRate->setCurrentItemByValue( pLFO->getFrequency() );
      }
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
   m_pcbWaveform->setVisible( pSample != nullptr );
   m_plRate->setVisible( pSample != nullptr );
   m_pcRate->setVisible( pSample != nullptr );
   m_pbSync->setVisible( pSample != nullptr );

   if( pSample )
   {
      m_pcbWaveform->setSelectedId( pSample->getLFO( getCurrentLFO() )->getWaveform(), dontSendNotification );
      SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
      m_pbSync->setToggleState( pLFO->getSyncEnabled(), dontSendNotification );

      if( pLFO->getSyncEnabled() )
      {
         m_pcRate->setItems( 0.1, 10.0, 0.1, "{:.1f}", "b" );
         m_pcRate->setCurrentItemByValue( pLFO->getSyncBeats() );
      } else
      {
         m_pcRate->setItems( 0.1, 10.0, 0.01, "{:.2f}", "Hz" );
         m_pcRate->setCurrentItemByValue( pLFO->getFrequency() );
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
