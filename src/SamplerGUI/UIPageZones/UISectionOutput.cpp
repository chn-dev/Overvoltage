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
\brief This class implements the output UI section
*/
/*----------------------------------------------------------------------------*/
#include "PluginEditor.h"
#include "UISectionOutput.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionOutput::UISectionOutput( UIPage *pUIPage ) :
   UISection( pUIPage, "Output" )
{
   m_pcbOutputBus = new juce::ComboBox( "Output Bus" );
   for( int i = 0; i < uiPage()->editor()->processor().getBusCount(false); i++ )
   {
      m_pcbOutputBus->addItem( uiPage()->editor()->processor().getBus( false, i )->getName(), i + 1 );
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionOutput::~UISectionOutput()
{
   delete m_plPan;
   delete m_psPan;
   delete m_plGain;
   delete m_psGain;
   delete m_pcbOutputBus;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::ComboBox::Listener
\param pComboBox The ComboBox that has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionOutput::comboBoxChanged( ComboBox *pComboBox )
{
   if( samples().size() > 0 )
   {
      if( pComboBox == m_pcbOutputBus )
      {
         for( SamplerEngine::Sample *pSample : samples() )
         {
            pSample->setOutputBus( m_pcbOutputBus->getSelectedId() - 1 );
         }
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Slider::Listener
\param pSlider The Slider that has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionOutput::sliderValueChanged( Slider *pSlider )
{
   if( samples().size() > 0 )
   {
      if( pSlider == m_psPan )
      {
         for( SamplerEngine::Sample *pSample : samples() )
         {
            pSample->setPan( (float)m_psPan->getValue() / 100.0f );
         }
      } else
      if( pSlider == m_psGain )
      {
         for( SamplerEngine::Sample *pSample : samples() )
         {
            pSample->setGain( powf( 10.0f, ( ( (float)m_psGain->getValue() ) / 20.0f ) ) );
         }
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionOutput::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionOutput::resized()
{
   UISection::resized();

   m_psPan->setBounds( 4, 32 - 4 + 32, 180, 25 );
   m_plPan->setBounds( 6, 48 - 4 + 32, 32, 16 );

   m_psGain->setBounds( 4, 64 - 4 + 32, 180, 25 );
   m_plGain->setBounds( 6, 80 - 4 + 32, 32, 16 );

   m_pcbOutputBus->setBounds( 4, 32 - 8, 180, 20 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the user has (de-)selected any samples
*/
/*----------------------------------------------------------------------------*/
void UISectionOutput::samplesUpdated()
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback frunction from juce::Label::Listener
\param pLabel The Label that has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionOutput::labelTextChanged( Label */*pLabel*/ )
{
}

