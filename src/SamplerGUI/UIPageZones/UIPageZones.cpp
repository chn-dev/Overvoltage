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
\file UIPageZones.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the "zones" UI page. This is actually the main page.
*/
/*----------------------------------------------------------------------------*/
#include <util.h>

#include "UIPageZones.h"
#include "PluginEditor.h"
#include "UISectionSamplerKeyboard.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UIPageZones::UIPageZones( PluginEditor *pEditor ) :
   UIPage( pEditor )
{
   setBounds( 0, 32, 1024, 640 - 32 );
   int yofs = 0;

   int kbWidth = 64 + 128;
   int xStart = kbWidth;
   int yStart = 196;
   int editorSectionWidth = 1024 - kbWidth;
   int editorSectionHeight = 640 - 32 - yStart;
   int margin = 2;

   for( int i = 0; i < SAMPLERENGINE_NUMLAYERS; i++ )
   {
      char tmp[32];
      sprintf( tmp, "%c", 'A' + i );
      juce::TextButton *pB = new juce::TextButton( tmp );
      m_LayerButtons.push_back( pB );
      pB->setBounds( i * 24 + 4, 0, 18, 18 );
      pB->setToggleable( true );
      pB->setClickingTogglesState( true );
      pB->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
      pB->addListener( this );
      addAndMakeVisible( pB );
   }

   m_pbSolo = new juce::TextButton( "Solo" );
   m_pbSolo->setToggleable( true );
   m_pbSolo->setClickingTogglesState( true );
   m_pbSolo->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbSolo->setBounds( 4, 24, 60, 18 );
   m_pbSolo->addListener( this );
   addAndMakeVisible( m_pbSolo );

   m_pbSelLayer = new juce::TextButton( "Sel. Layer" );
   m_pbSelLayer->setBounds( 68, 24, 60, 18 );
   m_pbSelLayer->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbSelLayer->addListener( this );
   addAndMakeVisible( m_pbSelLayer );

   m_pbSelAll = new juce::TextButton( "Sel. All" );
   m_pbSelAll->setBounds( 132, 24, 60, 18 );
   m_pbSelAll->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbSelAll->addListener( this );
   addAndMakeVisible( m_pbSelAll );

   int kbofs = yofs + 48;
   m_pUISectionKeyboard = new SamplerGUI::UISectionSamplerKeyboard( this );
   pEditor->addKeyListener( m_pUISectionKeyboard );
   m_pUISectionKeyboard->setBounds( 0, kbofs, kbWidth, 640 - kbofs );
   m_pUISectionKeyboard->addListener( &( editor()->processor() ) );
   m_pUISectionKeyboard->addSamplerKeyboardListener( &( editor()->processor() ) );
   m_pUISectionKeyboard->setWidth( 64 );
   editor()->processor().juce::MidiKeyboardState::addListener( m_pUISectionKeyboard );
   addAndMakeVisible( m_pUISectionKeyboard );

   m_pUISectionWaveView = new SamplerGUI::UISectionWaveView( this );
   m_pUISectionWaveView->setBounds( kbWidth + 4, yofs, 1024 - ( 64 + 128 + 4 ), yStart );
   addAndMakeVisible( m_pUISectionWaveView );


   // First row
   m_pUISectionSample = new SamplerGUI::UISectionSample( this );
   m_pUISectionSample->setBounds(
      xStart + margin,
      yStart + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionSample );

   m_pUISectionFilter = new SamplerGUI::UISectionFilter( this );
   m_pUISectionFilter->setBounds(
      xStart + ( editorSectionWidth / 4 ) + margin,
      yStart + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionFilter );

   m_pUISectionOutput = new SamplerGUI::UISectionOutput( this );
   m_pUISectionOutput->setBounds(
      xStart + ( ( 3 * editorSectionWidth ) / 4 ) + margin,
      yStart + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionOutput );

   // Second row
   m_pUISectionNameRanges = new SamplerGUI::UISectionNameRanges( this );
   m_pUISectionNameRanges->setBounds(
      xStart + margin,
      yStart + ( editorSectionHeight / 3 ) + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionNameRanges );

   m_pUISectionLFO = new SamplerGUI::UISectionLFO( this, "LFO" );
   m_pUISectionLFO->setBounds(
      xStart + ( ( editorSectionWidth / 4 ) * 1 ) + margin,
      yStart + ( editorSectionHeight / 3 ) + margin,
      ( ( editorSectionWidth * 2 ) / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionLFO );

   // Third row
   m_pUISectionAEG = new SamplerGUI::UISectionAEG( this );
   m_pUISectionAEG->setBounds(
      xStart + margin,
      yStart + ( ( 2 * editorSectionHeight ) / 3 ) + margin,
      ( editorSectionWidth / 8 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionAEG );

   m_pUISectionEG2 = new SamplerGUI::UISectionEG2( this );
   m_pUISectionEG2->setBounds(
      xStart + ( ( editorSectionWidth / 8 ) * 1 ) + margin,
      yStart + ( ( 2 * editorSectionHeight ) / 3 ) + margin,
      ( editorSectionWidth / 8 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionEG2 );

   m_pUISectionModMatrix = new SamplerGUI::UISectionModMatrix( this );
   m_pUISectionModMatrix->setBounds(
      xStart + ( ( editorSectionWidth / 4 ) * 1 ) + margin,
      yStart + ( ( 2 * editorSectionHeight ) / 3 ) + margin,
      ( ( editorSectionWidth * 3 ) / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionModMatrix );

   updateUISections();

   setCurrentLayer( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UIPageZones::~UIPageZones()
{
   delete m_pUISectionKeyboard;
   delete m_pUISectionWaveView;
   delete m_pUISectionSample;
   delete m_pUISectionNameRanges;
   delete m_pUISectionLFO;
   delete m_pUISectionAEG;
   delete m_pUISectionEG2;
   delete m_pUISectionFilter;
   delete m_pUISectionOutput;
   delete m_pUISectionModMatrix;

   for( size_t i = 0; i < m_LayerButtons.size(); i++ )
   {
      delete m_LayerButtons[i];
   }
   m_LayerButtons.clear();

   delete m_pbSelLayer;
   delete m_pbSelAll;
   delete m_pbSolo;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
\param pButton The button that has been clicked
*/
/*----------------------------------------------------------------------------*/
void UIPageZones::buttonClicked( Button *pButton )
{
   int nLayerButton = -1;
   for( size_t i = 0; i < m_LayerButtons.size(); i++ )
   {
      if( pButton == m_LayerButtons[i] )
      {
         nLayerButton = i;
         break;
      }
   }

   if( pButton == m_pbSelAll )
   {
      m_pUISectionKeyboard->selectAll();
   } else
   if( pButton == m_pbSelLayer )
   {
      m_pUISectionKeyboard->selectLayer( getCurrentLayer() );
   } else
   if( pButton == m_pbSolo )
   {
      //bool bSolo = m_pbSolo->getToggleState();
   } else
   if( nLayerButton >= 0 )
   {
      setCurrentLayer( nLayerButton );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Set the current layer.
\param nLayer The layer number (0..SAMPLERENGINE_NUMLAYERS-1)
*/
/*----------------------------------------------------------------------------*/
void UIPageZones::setCurrentLayer( int nLayer )
{
   if( nLayer < 0 || nLayer >= (int)m_LayerButtons.size() )
   {
      return;
   }

   for( size_t i = 0; i < m_LayerButtons.size(); i++ )
   {
      m_LayerButtons[i]->setToggleState( (int)i == nLayer, dontSendNotification );
   }

   repaint();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\retrn The currently selected layer number (0..SAMPLERENGINE_NUMLAYERS-1)
*/
/*----------------------------------------------------------------------------*/
int UIPageZones::getCurrentLayer() const
{
   for( size_t i = 0; i < m_LayerButtons.size(); i++ )
   {
      if( m_LayerButtons[i]->getToggleState() )
         return( (int)i );
   }

   return( -1 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
\param pButton The button whose state has changed
*/
/*----------------------------------------------------------------------------*/
void UIPageZones::buttonStateChanged( Button */*pButton*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UIPageZones::currentPartChanged( size_t /*nPart*/ )
{
//   m_pUISectionKeyboard->clearSelectedSamples();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The sampler keyboard
*/
/*----------------------------------------------------------------------------*/
SamplerGUI::UISectionSamplerKeyboard *UIPageZones::getSamplerKeyboard() const
{
   return( m_pUISectionKeyboard );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the solo mode is enabled
*/
/*----------------------------------------------------------------------------*/
bool UIPageZones::isSoloEnabled() const
{
   return( m_pbSolo->getToggleState() );
}

