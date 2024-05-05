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
   m_pcbWaveform->setBounds( 46, 22, 88, 18 );
   addAndMakeVisible( m_pcbWaveform );

   int yStep = 22;
   int yy = 44;

   // Rate
   m_plRate = new juce::Label( juce::String(), "Rate:" );
   m_plRate->setBounds( 0, yy, 48, 18 );
   addAndMakeVisible( m_plRate );

   m_pcRate = new CycleComponent();
   m_pcRate->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcRate->addListener( this );
   m_pcRate->setJustificationType( juce::Justification::centred );
   m_pcRate->setBounds( 46, yy + 3, 47, 13 );
   addAndMakeVisible( m_pcRate );

   m_pbRateSync = new juce::TextButton( "sync" );
   m_pbRateSync->setToggleable( true );
   m_pbRateSync->setClickingTogglesState( true );
   m_pbRateSync->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbRateSync->addListener( this );
   m_pbRateSync->setBounds( m_pcRate->getX() + m_pcRate->getWidth() + 10, yy + 2, 30, 16 );
   addAndMakeVisible( m_pbRateSync );

   // Delay
   m_plDelay = new juce::Label( juce::String(), "Delay:" );
   m_plDelay-> setBounds( m_plRate->getX(), m_plRate->getY() + yStep, m_plRate->getWidth(), m_plRate->getHeight());
   addAndMakeVisible( m_plDelay );

   m_pcDelay = new CycleComponent();
   m_pcDelay->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcDelay->addListener( this );
   m_pcDelay->setJustificationType( juce::Justification::centred );
   m_pcDelay->setBounds( m_pcRate->getX(), m_pcRate->getY() + yStep, m_pcRate->getWidth(), m_pcRate->getHeight() );
   addAndMakeVisible( m_pcDelay );

   m_pbDelaySync = new juce::TextButton( "sync" );
   m_pbDelaySync->setToggleable( true );
   m_pbDelaySync->setClickingTogglesState( true );
   m_pbDelaySync->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbDelaySync->addListener( this );
   m_pbDelaySync->setBounds( m_pbRateSync->getX(), m_pbRateSync->getY() + yStep, m_pbRateSync->getWidth(), m_pbRateSync->getHeight() );
   addAndMakeVisible( m_pbDelaySync );

   // Fade In
   m_plFadeIn = new juce::Label( juce::String(), "Fade In:" );
   m_plFadeIn->setBounds( m_plDelay->getX(), m_plDelay->getY() + yStep, m_plDelay->getWidth(), m_plDelay->getHeight());
   addAndMakeVisible( m_plFadeIn );

   m_pcFadeIn = new CycleComponent();
   m_pcFadeIn->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcFadeIn->addListener( this );
   m_pcFadeIn->setJustificationType( juce::Justification::centred );
   m_pcFadeIn->setBounds( m_pcDelay->getX(), m_pcDelay->getY() + yStep, m_pcDelay->getWidth(), m_pcDelay->getHeight() );
   addAndMakeVisible( m_pcFadeIn );

   m_pbFadeInSync = new juce::TextButton( "sync" );
   m_pbFadeInSync->setToggleable( true );
   m_pbFadeInSync->setClickingTogglesState( true );
   m_pbFadeInSync->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbFadeInSync->addListener( this );
   m_pbFadeInSync->setBounds( m_pbDelaySync->getX(), m_pbDelaySync->getY() + yStep, m_pbDelaySync->getWidth(), m_pbDelaySync->getHeight() );
   addAndMakeVisible( m_pbFadeInSync );

   // Once
   m_pbOnce = new juce::TextButton( "once" );
   m_pbOnce->setToggleable( true );
   m_pbOnce->setClickingTogglesState( true );
   m_pbOnce->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbOnce->addListener( this );
   m_pbOnce->setBounds( 16, m_pbFadeInSync->getY() + yStep, m_pbFadeInSync->getWidth() * 7 / 4, m_pbFadeInSync->getHeight() );
   addAndMakeVisible( m_pbOnce );

   // Random phase
   m_pbRndPhase = new juce::TextButton( "rndphase" );
   m_pbRndPhase->setToggleable( true );
   m_pbRndPhase->setClickingTogglesState( true );
   m_pbRndPhase->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbRndPhase->addListener( this );
   m_pbRndPhase->setBounds( m_pbOnce->getX() + m_pbOnce->getWidth() + 4, m_pbOnce->getY(), m_pbOnce->getWidth(), m_pbOnce->getHeight());
   addAndMakeVisible( m_pbRndPhase );


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
   delete m_pbRateSync;
   delete m_plDelay;
   delete m_pcDelay;
   delete m_pbDelaySync;
   delete m_plFadeIn;
   delete m_pcFadeIn;
   delete m_pbFadeInSync;
   delete m_pbOnce;
   delete m_pbRndPhase;
}


void UISectionLFO::labelTextChanged( Label *pLabel )
{
   if( m_pcRate == pLabel )
   {
      double value = m_pcRate->getCurrentItemValue();
      size_t nLFO = getCurrentLFO();
      bool syncEnabled = m_pbRateSync->getToggleState();
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
   } else
   if( m_pcDelay == pLabel )
   {
      double value = m_pcDelay->getCurrentItemValue();
      size_t nLFO = getCurrentLFO();
      bool syncEnabled = m_pbDelaySync->getToggleState();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         if( pSample->getLFO( nLFO )->getDelaySyncEnabled() == syncEnabled )
         {
            if( syncEnabled )
            {
               pSample->getLFO( nLFO )->setDelayBeats( value );
            } else
            {
               pSample->getLFO( nLFO )->setDelaySecs( value );
            }
         }
      }
   } else
   if( m_pcFadeIn == pLabel )
   {
      double value = m_pcFadeIn->getCurrentItemValue();
      size_t nLFO = getCurrentLFO();
      bool syncEnabled = m_pbFadeInSync->getToggleState();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         if( pSample->getLFO( nLFO )->getFadeInSyncEnabled() == syncEnabled )
         {
            if( syncEnabled )
            {
               pSample->getLFO( nLFO )->setFadeInBeats( value );
            } else
            {
               pSample->getLFO( nLFO )->setFadeInSecs( value );
            }
         }
      }
   }
}


void UISectionLFO::updateInfo()
{
   SamplerEngine::Sample *pSample = sample();
   if( !pSample )
      return;

   size_t nLFO = getCurrentLFO();
   SamplerEngine::LFO *pLFO = pSample->getLFO( nLFO );

   m_pcbWaveform->setSelectedId( pLFO->getWaveform(), dontSendNotification );

   m_pbRateSync->setToggleState( pLFO->getSyncEnabled(), dontSendNotification );
   if( pLFO->getSyncEnabled() )
   {
      m_pcRate->setItems( 0.1, 10.0, 0.1, "{:.1f}", "b" );
      m_pcRate->setCurrentItemByValue( pLFO->getSyncBeats() );
   } else
   {
      m_pcRate->setItems( 0.01, 10.0, 0.01, "{:.2f}", "Hz" );
      m_pcRate->setCurrentItemByValue( pLFO->getFrequency() );
   }

   m_pbDelaySync->setToggleState( pLFO->getDelaySyncEnabled(), dontSendNotification );
   if( pLFO->getDelaySyncEnabled() )
   {
      m_pcDelay->setItems( 0.0, 10.0, 0.1, "{:.1f}", "b" );
      m_pcDelay->setCurrentItemByValue( pLFO->getDelayBeats() );
   } else
   {
      m_pcDelay->setItems( 0.0, 10.0, 0.01, "{:.2f}", "s" );
      m_pcDelay->setCurrentItemByValue( pLFO->getDelaySecs() );
   }

   m_pbFadeInSync->setToggleState( pLFO->getFadeInSyncEnabled(), dontSendNotification );
   if( pLFO->getFadeInSyncEnabled() )
   {
      m_pcFadeIn->setItems( 0.0, 10.0, 0.1, "{:.1f}", "b" );
      m_pcFadeIn->setCurrentItemByValue( pLFO->getFadeInBeats() );
   } else
   {
      m_pcFadeIn->setItems( 0.0, 10.0, 0.01, "{:.2f}", "s" );
      m_pcFadeIn->setCurrentItemByValue( pLFO->getFadeInSecs() );
   }

   m_pbOnce->setToggleState( pLFO->getOnceEnabled(), dontSendNotification );
   m_pbRndPhase->setToggleState( pLFO->getRandomPhaseEnabled(), dontSendNotification );
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


   if( pButton == m_pbRateSync )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
         pLFO->setSyncEnabled( m_pbRateSync->getToggleState() );
      }

      updateInfo();
   } else
   if( pButton == m_pbDelaySync )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
         pLFO->setDelaySyncEnabled( m_pbDelaySync->getToggleState() );
      }

      updateInfo();
   } else
   if( pButton == m_pbFadeInSync )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
         pLFO->setFadeInSyncEnabled( m_pbFadeInSync->getToggleState() );
      }

      updateInfo();
   } else
   if( pButton == m_pbOnce )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
         pLFO->setOnceEnabled( m_pbOnce->getToggleState() );
      }

      updateInfo();
   } else
   if( pButton == m_pbRndPhase )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         SamplerEngine::LFO *pLFO = pSample->getLFO( getCurrentLFO() );
         pLFO->setRandomPhaseEnabled( m_pbRndPhase->getToggleState() );
      }

      updateInfo();
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
   m_pbRateSync->setVisible( pSample != nullptr );
   m_plDelay->setVisible( pSample != nullptr );
   m_pcDelay->setVisible( pSample != nullptr );
   m_pbDelaySync->setVisible( pSample != nullptr );
   m_plFadeIn->setVisible( pSample != nullptr );
   m_pcFadeIn->setVisible( pSample != nullptr );
   m_pbFadeInSync->setVisible( pSample != nullptr );
   m_pbOnce->setVisible( pSample != nullptr );
   m_pbRndPhase->setVisible( pSample != nullptr );

   if( pSample )
   {
      updateInfo();
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
