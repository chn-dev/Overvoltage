/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of chn's Overvoltage.                                   *
 *                                                                             *
 *  Overvoltage is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *          
 *  Overvoltage is distributed in the hope that it will be useful, but         * 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Overvoltage. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file UISectionLFO.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the LFO UI section
*/
/*----------------------------------------------------------------------------*/
#include <util.h>
#include "UISectionLFO.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionLFO::StepEditor::StepEditor( UISectionLFO *pSectionLFO ) :
   m_pSectionLFO( pSectionLFO )
{
   m_plNumSteps = new juce::Label( juce::String(), "Steps:" );
   m_plNumSteps->setBounds( 0, 91, 38, 18 );
   addAndMakeVisible( m_plNumSteps );

   m_pcNumSteps = new CycleComponent();
   m_pcNumSteps->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcNumSteps->setJustificationType( juce::Justification::centred );
   m_pcNumSteps->setBounds( 38, 91, 32, 18 );
   m_pcNumSteps->setItems( 1.0, 128, 1.0, "{:.0f}", "" );
   m_pcNumSteps->addListener( this );
   addAndMakeVisible( m_pcNumSteps );

   m_pbQuantize = new juce::TextButton( "Quantize:" );
   m_pbQuantize->setToggleable( true );
   m_pbQuantize->setClickingTogglesState( true );
   m_pbQuantize->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbQuantize->addListener( this );
   m_pbQuantize->setBounds( 78, 91, 48, 18 );
   addAndMakeVisible( m_pbQuantize );

   m_pcQuantize = new CycleComponent();
   m_pcQuantize->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcQuantize->setJustificationType( juce::Justification::centred );
   m_pcQuantize->setBounds( 130, 91, 32, 18 );
   m_pcQuantize->setItems( 1.0, 32, 1.0, "{:.0f}", "" );
   m_pcQuantize->addListener( this );
   addAndMakeVisible( m_pcQuantize );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionLFO::StepEditor::~StepEditor()
{
   delete m_plNumSteps;
   delete m_pcNumSteps;
   delete m_pbQuantize;
   delete m_pcQuantize;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
int UISectionLFO::StepEditor::getDiagramXPos() const
{
   return( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
int UISectionLFO::StepEditor::getDiagramYPos() const
{
   return( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
int UISectionLFO::StepEditor::getDiagramWidth() const
{
   return( getBounds().getWidth() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
int UISectionLFO::StepEditor::getDiagramHeight() const
{
   return( getBounds().getHeight() - 20 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::update()
{
   SamplerEngine::LFO *pLFO = m_pSectionLFO->getLFO();
   if( pLFO )
   {
      m_pcNumSteps->setCurrentItemByValue( (double)pLFO->getCustomRef().size() );
      m_pbQuantize->setToggleState( pLFO->getCustomQuantizeEnabled(), dontSendNotification );
      m_pcQuantize->setCurrentItemByValue( (double)pLFO->getCustomQuantize() );
   }

   repaint();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::buttonClicked( Button *pButton )
{
   if( m_pbQuantize == pButton )
   {
      SamplerEngine::LFO *pLFO = m_pSectionLFO->getLFO();
      if( pLFO )
      {
         pLFO->setCustomQuantizeEnabled( m_pbQuantize->getToggleState() );
         update();
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::buttonStateChanged( Button */*pButton*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Label::Listener
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::labelTextChanged( Label *pLabel )
{
   if( pLabel == m_pcNumSteps )
   {
      SamplerEngine::LFO *pLFO = m_pSectionLFO->getLFO();
      if( pLFO )
      {
         size_t s = (size_t)( m_pcNumSteps->getCurrentItemValue() + 0.5 );
         if( s != pLFO->getCustomRef().size() )
         {
            pLFO->getCustomRef().resize( s );
            update();
         }
      }
   } else
   if( pLabel == m_pcQuantize )
   {
      SamplerEngine::LFO *pLFO = m_pSectionLFO->getLFO();
      if( pLFO )
      {
         size_t q = (size_t)( m_pcQuantize->getCurrentItemValue() + 0.5 );
         if( q != pLFO->getCustomQuantize() )
         {
            pLFO->setCustomQuantize( q );
            update();
         }
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();

   g.setColour( juce::Colour::fromRGB( 16, 32, 16 ) );
   g.fillRect( getDiagramXPos(), getDiagramYPos(), getDiagramWidth(), getDiagramHeight() );

   SamplerEngine::LFO *pLFO = m_pSectionLFO->getLFO();
   if( !pLFO )
   {
      return;
   }

   for( size_t i = 0; i < pLFO->getNumCustomValues(); i++ )
   {
      double v = pLFO->getCustomValue( i );

      int x = (int)( ( (double)( (int)i * getDiagramWidth() ) / (double)pLFO->getNumCustomValues() ) + 0.5 );
      int y = getDiagramHeight() / 2;
      int x2 = (int)( ( (double)( ( (int)i + 1 ) * getDiagramWidth() ) / (double)pLFO->getNumCustomValues() ) + 0.5 );
      int w = x2 - x;
      int h = (int)( - v * getDiagramHeight() / 2.0 );

      if( h < 0 )
      {
         h = -h;
         y -= h;
      }

      g.setColour( juce::Colour::fromRGB( 96, 96, 96 ) );
      g.fillRect( x + getDiagramXPos(), y + getDiagramYPos(), w, h );

      g.setColour( juce::Colour::fromRGB( 48, 48, 48 ) );
      g.drawVerticalLine( x + getDiagramXPos(), (float)getDiagramYPos(), (float)( getDiagramYPos() + getDiagramHeight() - 1 ) );
   }

   if( pLFO->getCustomQuantizeEnabled() )
   {
      g.setColour( juce::Colour::fromRGB( 48, 48, 48 ) );
      for( size_t i = 1; i <= pLFO->getCustomQuantize(); i++ )
      {
         int ybase = getDiagramHeight() / 2;
         int yofs = (int)( ( (int)i * getDiagramHeight() ) / ( 2 * (int)pLFO->getCustomQuantize() ) );
         g.drawHorizontalLine( ybase + yofs, (float)getDiagramXPos(), (float)( getDiagramXPos() + getDiagramWidth() ) );
         g.drawHorizontalLine( ybase - yofs, (float)getDiagramXPos(), (float)( getDiagramXPos() + getDiagramWidth() ) );
      }
   }

   for( size_t i = 0; i < pLFO->getNumCustomValues(); i++ )
   {
      double v = pLFO->getCustomValue( i );;

      int x = (int)( ( (double)( (int)i * getDiagramWidth() ) / (double)pLFO->getNumCustomValues() ) + 0.5 );
      int y = getDiagramHeight() / 2;
      int x2 = (int)( ( (double)( ( (int)i + 1 ) * getDiagramWidth() ) / (double)pLFO->getNumCustomValues() ) + 0.5 );
      int w = x2 - x;
      int h = (int)( - v * getDiagramHeight() / 2.0 );

      int yy = y + h;

      g.setColour( juce::Colour::fromRGB( 255, 32, 32 ) );
      g.drawHorizontalLine( yy + getDiagramYPos(), (float)( x + getDiagramXPos() ), (float)( x + w + getDiagramXPos() ) );
   }

   g.setColour( juce::Colour::fromRGB( 128, 128, 128 ) );
   g.drawRect( getDiagramXPos(), getDiagramYPos(), getDiagramWidth(), getDiagramHeight() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::mouseMove( const MouseEvent &/*event*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::mouseDrag( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();
   changeStepValue( x, y );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::mouseDown( const MouseEvent &event )
{
   int x = event.getMouseDownPosition().getX();
   int y = event.getMouseDownPosition().getY();
   changeStepValue( x, y );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::mouseUp( const MouseEvent &/*event*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Change one step value of the custom waveform
\param x
\param y
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::StepEditor::changeStepValue( int x, int y )
{
   SamplerEngine::LFO *pLFO = m_pSectionLFO->getLFO();
   if( !pLFO )
      return;

   std::vector<double> &steps = pLFO->getCustomRef();

   int nStep = ( ( x - getDiagramXPos() ) * (int)steps.size() ) / getDiagramWidth();
   double v = ( ( (double)( y - getDiagramYPos() ) / (double)( getDiagramHeight() - 1 ) ) * 2.0 ) - 1.0;
   v = -v;
   if( nStep < 0 || nStep >= (int)steps.size() ||
       v < -1.0 || v > 1.0 )
      return;

   steps[(size_t)nStep] = v;
   repaint();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The LFO
*/
/*----------------------------------------------------------------------------*/
SamplerEngine::LFO *UISectionLFO::getLFO() const
{
   if( !sample() )
   {
      return( nullptr );
   }

   return( sample()->getLFO( getCurrentLFO() ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
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
      pB->setBounds( 2 + 18 * (int)i, 1, 16, 16 );
      addAndMakeVisible( pB );
      m_SelButtons.push_back( pB );
   }

   m_plWaveform = new juce::Label( juce::String(), "Wave:" );
   m_plWaveform->setBounds( 0, 22, 48, 18 );
   addAndMakeVisible( m_plWaveform );

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

   m_pStepEditor = new StepEditor( this );
   m_pStepEditor->setBounds( 138, 22, 272, 109 );
   addAndMakeVisible( m_pStepEditor );

   setCurrentLFO( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionLFO::~UISectionLFO()
{
   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      delete m_SelButtons[i];
   }
   m_SelButtons.clear();

   delete m_pcbWaveform;
   delete m_plWaveform;
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
   delete m_pStepEditor;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Label::Listener
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::updateInfo()
{
   SamplerEngine::Sample *pSample = sample();
   if( !pSample )
      return;

   size_t nLFO = getCurrentLFO();
   SamplerEngine::LFO *pLFO = pSample->getLFO( nLFO );

   m_pStepEditor->update();
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
\param pButton The button that has been clicked
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
\param pButton The button whose state has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::buttonStateChanged( Button */*pButton*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::resized()
{
   UISection::resized();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
The user has (de-)selected some samples
*/
/*----------------------------------------------------------------------------*/
void UISectionLFO::samplesUpdated()
{
   const SamplerEngine::Sample *pSample = sample();

   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      m_SelButtons[i]->setVisible( pSample != nullptr );
   }
   m_pcbWaveform->setVisible( pSample != nullptr );
   m_plWaveform->setVisible( pSample != nullptr );
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
   m_pStepEditor->setVisible( pSample != nullptr );

   if( pSample )
   {
      updateInfo();
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Set the current LFO
\param n The number of the LFO to be selected (0..NUM_LFO-1)
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of the currently selected LFO (0..NUM_LFO-1)
*/
/*----------------------------------------------------------------------------*/
size_t UISectionLFO::getCurrentLFO() const
{
   for( size_t i = 0; i < m_SelButtons.size(); i++ )
   {
      if( m_SelButtons[i]->getToggleState() )
         return( i );
   }

   return( (size_t)-1 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::ComboBox::Listener
\param pComboBox The ComboBox that has changed
*/
/*----------------------------------------------------------------------------*/
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

