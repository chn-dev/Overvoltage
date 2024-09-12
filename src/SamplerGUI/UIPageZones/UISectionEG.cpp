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
\file UISection.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a general EG UI section
*/
/*----------------------------------------------------------------------------*/
#include "UISectionEG.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionEG::UISectionEG( UIPage *pUIPage, std::string label ) :
   UISection( pUIPage, label )
{
   m_psAttack = new juce::Slider( "Attack" );
   m_psAttack->setRange( 0.0, 1.0, 0.001 );
   m_psAttack->setSliderStyle( juce::Slider::LinearVertical );
   m_psAttack->setSliderSnapsToMousePosition( false );
   m_psAttack->setLookAndFeel( &m_SliderLAF );
   m_psAttack->addListener( this );
   addAndMakeVisible( m_psAttack );
   m_plAttack = new juce::Label( juce::String(), "A" );
   addAndMakeVisible( m_plAttack );

   m_psDecay = new juce::Slider( "Decay" );
   m_psDecay->setRange( 0.0, 1.0, 0.001 );
   m_psDecay->setSliderStyle( juce::Slider::LinearVertical );
   m_psDecay->setSliderSnapsToMousePosition( false );
   m_psDecay->setLookAndFeel( &m_SliderLAF );
   m_psDecay->addListener( this );
   addAndMakeVisible( m_psDecay );
   m_plDecay = new juce::Label( juce::String(), "D" );
   addAndMakeVisible( m_plDecay );

   m_psSustain = new juce::Slider( "Sustain" );
   m_psSustain->setRange( 0.0, 1.0, 0.001 );
   m_psSustain->setSliderStyle( juce::Slider::LinearVertical );
   m_psSustain->setSliderSnapsToMousePosition( false );
   m_psSustain->setLookAndFeel( &m_SliderLAF );
   m_psSustain->addListener( this );
   addAndMakeVisible( m_psSustain );
   m_plSustain = new juce::Label( juce::String(), "S" );
   addAndMakeVisible( m_plSustain );

   m_psRelease = new juce::Slider( "Release" );
   m_psRelease->setRange( 0.0, 1.0, 0.001 );
   m_psRelease->setSliderStyle( juce::Slider::LinearVertical );
   m_psRelease->setSliderSnapsToMousePosition( false );
   m_psRelease->setLookAndFeel( &m_SliderLAF );
   m_psRelease->addListener( this );
   addAndMakeVisible( m_psRelease );
   m_plRelease = new juce::Label( juce::String(), "R" );
   addAndMakeVisible( m_plRelease );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionEG::~UISectionEG()
{
   delete m_psAttack;
   delete m_plAttack;
   delete m_psDecay;
   delete m_plDecay;
   delete m_psSustain;
   delete m_plSustain;
   delete m_psRelease;
   delete m_plRelease;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionEG::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionEG::resized()
{
   UISection::resized();

   m_psAttack->setBounds( 8, 16, 14, 100 );
   m_plAttack->setBounds( 5, 108, 18, 16 );

   m_psDecay->setBounds( 28, 16, 14, 100 );
   m_plDecay->setBounds( 25, 108, 18, 16 );

   m_psSustain->setBounds( 48, 16, 14, 100 );
   m_plSustain->setBounds( 45, 108, 18, 16 );

   m_psRelease->setBounds( 69, 16, 14, 100 );
   m_plRelease->setBounds( 66, 108, 18, 16 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the envelope generator's settings have changed. Adjusts the UI 
components accordingly.
\param *pENV The ENV whose settings have changed
*/
/*----------------------------------------------------------------------------*/
void UISectionEG::egUpdated( SamplerEngine::ENV *pENV )
{
   if( !pENV )
      return;

   m_psAttack->setValue( pENV->getAttack(), dontSendNotification );
   m_psDecay->setValue( pENV->getDecay(), dontSendNotification );
   m_psSustain->setValue( pENV->getSustain(), dontSendNotification );
   m_psRelease->setValue( pENV->getRelease(), dontSendNotification );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the user has (de-)selected any samples.
*/
/*----------------------------------------------------------------------------*/
void UISectionEG::samplesUpdated()
{
   if( sample() )
   {
      egUpdated( getENV( sample() ) );
   }

   m_psAttack->setVisible( sample() != nullptr );
   m_plAttack->setVisible( sample() != nullptr );
   m_psDecay->setVisible( sample() != nullptr );
   m_plDecay->setVisible( sample() != nullptr );
   m_psSustain->setVisible( sample() != nullptr );
   m_plSustain->setVisible( sample() != nullptr );
   m_psRelease->setVisible( sample() != nullptr );
   m_plRelease->setVisible( sample() != nullptr );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Slider::Listener
\param pSlider The Slider which has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionEG::sliderValueChanged( Slider *pSlider )
{
   if( samples().size() <= 0 )
      return;

   if( pSlider == m_psAttack )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setAttack( m_psAttack->getValue() );
      }
   } else
   if( pSlider == m_psDecay )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setDecay( m_psDecay->getValue() );
      }
   } else
   if( pSlider == m_psSustain )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setSustain( m_psSustain->getValue() );
      }
   } else
   if( pSlider == m_psRelease )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setRelease( m_psRelease->getValue() );
      }
   }
}

