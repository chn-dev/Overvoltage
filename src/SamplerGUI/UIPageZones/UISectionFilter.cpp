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
\brief This class implements the filter UI section
*/
/*----------------------------------------------------------------------------*/
#include "PluginEditor.h"
#include "UISectionFilter.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionFilter::UISectionFilter( UIPage *pUIPage ) :
   UISection( pUIPage, "Filter" )
{
   m_psCutoff = new juce::Slider( "Cutoff" );
   m_psCutoff->setRange( 0.0, 1.0, 0.001 );
   m_psCutoff->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psCutoff->setSliderSnapsToMousePosition( false );
   m_psCutoff->setLookAndFeel( &m_SliderLAF );
   m_psCutoff->addListener( this );
   m_psCutoff->setTextBoxStyle( juce::Slider::NoTextBox, true, 1, 1 );
   addAndMakeVisible( m_psCutoff );
   m_plCutoff = new juce::Label( juce::String(), "Cutoff:" );
   addAndMakeVisible( m_plCutoff );

   m_psResonance = new juce::Slider( "Resonance" );
   m_psResonance->setRange( 0.0, 1.0, 0.001 );
   m_psResonance->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psResonance->setSliderSnapsToMousePosition( false );
   m_psResonance->setLookAndFeel( &m_SliderLAF );
   m_psResonance->addListener( this );
   m_psResonance->setTextBoxStyle( juce::Slider::NoTextBox, true, 1, 1 );
   addAndMakeVisible( m_psResonance );
   m_plResonance = new juce::Label( juce::String(), "Reso:" );
   addAndMakeVisible( m_plResonance );

   m_pcbType = new juce::ComboBox( "Type" );
   for( SamplerEngine::Filter::Type type : SamplerEngine::Filter::allTypes() )
   {
      m_pcbType->addItem( SamplerEngine::Filter::toString( type ), type );
   }
   m_pcbType->addListener( this );
   addAndMakeVisible( m_pcbType );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionFilter::~UISectionFilter()
{
   delete m_psCutoff;
   delete m_plCutoff;
   delete m_psResonance;
   delete m_plResonance;
   delete m_pcbType;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionFilter::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionFilter::resized()
{
   UISection::resized();

   m_psCutoff->setBounds( 48, 64, 150, 16 );
   m_plCutoff->setBounds( 4, 64, 64, 12 );

   m_psResonance->setBounds( 48, 96, 150, 16 );
   m_plResonance->setBounds( 4, 96, 64, 12 );

   m_pcbType->setBounds( 4, 32, 180, 16 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Gets called when the filter settings have changed. Updates the UI components 
accordingly.
\param pFilter The filter
*/
/*----------------------------------------------------------------------------*/
void UISectionFilter::filterUpdated( SamplerEngine::Filter *pFilter )
{
   m_psCutoff->setValue( pFilter->getCutoff(), dontSendNotification );
   m_psResonance->setValue( pFilter->getResonance(), dontSendNotification );
   m_pcbType->setSelectedId( pFilter->getType() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionFilter::samplesUpdated()
{
   const SamplerEngine::Sample *pSample = sample();
   if( pSample )
   {
      filterUpdated( pSample->getFilter() );
   }

   m_plCutoff->setVisible( pSample != nullptr );
   m_psCutoff->setVisible( pSample != nullptr );
   m_plResonance->setVisible( pSample != nullptr );
   m_psResonance->setVisible( pSample != nullptr );
   m_pcbType->setVisible( pSample != nullptr );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::ComboBox::Listener
\param pComboBox The ComboBox which has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionFilter::comboBoxChanged( ComboBox *pComboBox )
{
   if( pComboBox == m_pcbType )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         pSample->getFilter()->setType( (SamplerEngine::Filter::Type)m_pcbType->getSelectedId() );
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Slider::listener
\param pSlider The Slider which has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionFilter::sliderValueChanged( Slider *pSlider )
{
   if( samples().size() <= 0 )
      return;

   if( m_psCutoff == pSlider )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         pSample->getFilter()->setCutoff( m_psCutoff->getValue() );
      }
   } else
   if( m_psResonance == pSlider )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         pSample->getFilter()->setResonance( m_psResonance->getValue() );
      }
   }
}

