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
\file UISectionNameRanges.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the names&ranges UI section
*/
/*----------------------------------------------------------------------------*/
#include "PluginEditor.h"
#include "UISectionNameRanges.h"

#include "util.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionNameRanges::UISectionNameRanges( UIPage *pUIPage ) :
   UISection( pUIPage, "Name & Ranges" )
{
   std::vector<std::string> noteNames;
   for( int i = 0; i < 128; i++ )
   {
      noteNames.push_back( UISectionKeyboard::noteNameWithOctave( i ).toStdString() );
   }

   int d = 20;

   m_plLow = new juce::Label( juce::String(), "low" );
   m_plLow->setBounds( 32, 32 + d, 32, 14 );
   addAndMakeVisible( m_plLow );
   m_plHigh = new juce::Label( juce::String(), "hi" );
   m_plHigh->setBounds( 128, 32 + d, 32, 14 );
   addAndMakeVisible( m_plHigh );
   m_plBaseNote = new juce::Label( juce::String(), "root" );
   m_plBaseNote->setBounds( 80, 32 + d, 32, 14 );
   addAndMakeVisible( m_plBaseNote );
   m_plK1 = new juce::Label( juce::String(), "K" );
   m_plK1->setBounds( 0, 48 + d, 18, 14 );
   addAndMakeVisible( m_plK1 );
   m_plK2 = new juce::Label( juce::String(), "K" );
   m_plK2->setBounds( getBounds().getWidth() - 18, 48 + d, 18, 14 );
   addAndMakeVisible( m_plK2 );
   m_plV1 = new juce::Label( juce::String(), "V" );
   m_plV1->setBounds( 0, 68 + d, 18, 14 );
   addAndMakeVisible( m_plV1 );
   m_plV2 = new juce::Label( juce::String(), "V" );
   m_plV2->setBounds( getBounds().getWidth() - 18, 68 + d, 18, 14 );
   addAndMakeVisible( m_plV2 );

   m_plKeytrack = new juce::Label( juce::String(), "Keytrack:" );
   m_plKeytrack->setBounds( 0, 88 + d, 56, 14 );
   addAndMakeVisible( m_plKeytrack );

   m_pcKeytrack = new CycleComponent();
   m_pcKeytrack->setItems( -400.0, 400.0, 1.0, "{:.0f}", "%" );
   m_pcKeytrack->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcKeytrack->addListener( this );
   m_pcKeytrack->setJustificationType( juce::Justification::centred );
   m_pcKeytrack->setBounds( 52, 88 + d, 52, 14 );
   addAndMakeVisible( m_pcKeytrack );

   m_plPitchbendRange = new juce::Label( juce::String(), "PB range:" );
   m_plPitchbendRange->setBounds( 52 + 52, 88 + d, 56, 14 );
   addAndMakeVisible( m_plPitchbendRange );

   m_pcPitchbendRange = new CycleComponent();
   m_pcPitchbendRange->setItems( -12.0, 12.0, 0.1, "{:.1f}", "st" );
   m_pcPitchbendRange->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcPitchbendRange->addListener( this );
   m_pcPitchbendRange->setJustificationType( juce::Justification::centred );
   m_pcPitchbendRange->setBounds( 52 + 52 + 52, 88 + d, 44, 14 );
   addAndMakeVisible( m_pcPitchbendRange );

   m_pcMinNote = new CycleComponent( noteNames );
   m_pcMinNote->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMinNote->addListener( this );
   m_pcMinNote->setJustificationType( juce::Justification::centred );
   m_pcMinNote->setBounds( 32, 48 + d, 38, 14 );
   addAndMakeVisible( m_pcMinNote );

   m_pcBaseNote = new CycleComponent( noteNames );
   m_pcBaseNote->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcBaseNote->addListener( this );
   m_pcBaseNote->setJustificationType( juce::Justification::centred );
   m_pcBaseNote->setBounds( 80, 48 + d, 38, 14 );
   addAndMakeVisible( m_pcBaseNote );

   m_pcMaxNote = new CycleComponent( noteNames );
   m_pcMaxNote->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMaxNote->addListener( this );
   m_pcMaxNote->setJustificationType( juce::Justification::centred );
   m_pcMaxNote->setBounds( 128, 48 + d, 38, 14 );
   addAndMakeVisible( m_pcMaxNote );

   std::vector<std::string> velNames;
   for( int i = 0; i < 128; i++ )
   {
      velNames.push_back( stdformat( "{}", i ) );
   }
   m_pcMinVelocity = new CycleComponent( velNames );
   m_pcMinVelocity->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMinVelocity->addListener( this );
   m_pcMinVelocity->setJustificationType( juce::Justification::centred );
   m_pcMinVelocity->setBounds( 32, 68 + d, 38, 14 );
   addAndMakeVisible( m_pcMinVelocity );

   m_pcMaxVelocity = new CycleComponent( velNames );
   m_pcMaxVelocity->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMaxVelocity->addListener( this );
   m_pcMaxVelocity->setJustificationType( juce::Justification::centred );
   m_pcMaxVelocity->setBounds( 128, 68 + d, 38, 14 );
   addAndMakeVisible( m_pcMaxVelocity );

   m_plName = new juce::Label( juce::String(), "" );
   m_plName->addListener( this );
   m_plName->setJustificationType( juce::Justification::left );
   m_plName->setEditable( true );
   m_plName->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_plName->setColour( juce::Label::ColourIds::outlineWhenEditingColourId, juce::Colour::fromRGBA( 255, 255, 255, 128 ) );
   m_plName->addListener( this );
   m_plName->setBounds( 8, 28, 188, 18 );
   addAndMakeVisible( m_plName );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionNameRanges::~UISectionNameRanges()
{
   delete m_pcMinNote;
   delete m_pcMaxNote;
   delete m_pcMinVelocity;
   delete m_pcMaxVelocity;
   delete m_plLow;
   delete m_plHigh;
   delete m_plK1;
   delete m_plK2;
   delete m_plV1;
   delete m_plV2;
   delete m_pcKeytrack;
   delete m_plKeytrack;
   delete m_pcPitchbendRange;
   delete m_plPitchbendRange;
   delete m_plBaseNote;
   delete m_pcBaseNote;
   delete m_plName;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Label::Listener
\param pLabel The Label that has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionNameRanges::labelTextChanged( Label *pLabel )
{
   if( samples().size() != 1 )
      return;

   if( pLabel == m_plName )
   {
      sample()->setName( m_plName->getText().toStdString() );
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMinVelocity )
   {
      sample()->setMinVelocity( m_pcMinVelocity->getCurrentItem() );
      if( sample()->getMaxVelocity() < sample()->getMinVelocity() )
      {
         sample()->setMaxVelocity( sample()->getMinVelocity() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMaxVelocity )
   {
      sample()->setMaxVelocity( m_pcMaxVelocity->getCurrentItem() );
      if( sample()->getMinVelocity() > sample()->getMaxVelocity() )
      {
         sample()->setMinVelocity( sample()->getMaxVelocity() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMinNote )
   {
      sample()->setMinNote( m_pcMinNote->getCurrentItem() );
      if( sample()->getMaxNote() < sample()->getMinNote() )
      {
         sample()->setMaxNote( sample()->getMinNote() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMaxNote )
   {
      sample()->setMaxNote( m_pcMaxNote->getCurrentItem() );
      if( sample()->getMinNote() > sample()->getMaxNote() )
      {
         sample()->setMinNote( sample()->getMaxNote() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcBaseNote )
   {
      sample()->setBaseNote( m_pcBaseNote->getCurrentItem() );
   } else
   if( pLabel == m_pcKeytrack )
   {
      sample()->setKeytrack( (float)m_pcKeytrack->getCurrentItem() - 400.0f );
   } else
   if( pLabel == m_pcPitchbendRange )
   {
      sample()->setPitchbendRange( ( (float)m_pcPitchbendRange->getCurrentItem() * 0.1f ) - 12.0f );
   }
   uiPage()->editor()->repaint();

   samplesUpdated();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionNameRanges::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionNameRanges::resized()
{
   UISection::resized();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the user has (de-)selected any samples
*/
/*----------------------------------------------------------------------------*/
void UISectionNameRanges::samplesUpdated()
{
   bool ok = samples().size() == 1;

   m_pcMinNote->setVisible( ok );
   m_pcMaxNote->setVisible( ok );
   m_pcMinVelocity->setVisible( ok );
   m_pcMaxVelocity->setVisible( ok );
   m_plLow->setVisible( ok );
   m_plHigh->setVisible( ok );
   m_plK1->setVisible( ok );
   m_plK2->setVisible( ok );
   m_plV1->setVisible( ok );
   m_plV2->setVisible( ok );
   m_plBaseNote->setVisible( ok );
   m_pcBaseNote->setVisible( ok );
   m_plName->setVisible( ok );
   m_plKeytrack->setVisible( ok );
   m_pcKeytrack->setVisible( ok );
   m_plPitchbendRange->setVisible( ok );
   m_pcPitchbendRange->setVisible( ok );

   if( ok )
   {
      m_pcMinNote->setCurrentItem( sample()->getMinNote() );
      m_pcMaxNote->setCurrentItem( sample()->getMaxNote() );
      m_pcBaseNote->setCurrentItem( sample()->getBaseNote() );
      m_pcMinVelocity->setCurrentItem( sample()->getMinVelocity() );
      m_pcMaxVelocity->setCurrentItem( sample()->getMaxVelocity() );
      m_plName->setText( sample()->getName(), juce::dontSendNotification );
      m_pcKeytrack->setCurrentItem( (int)( sample()->getKeytrack() + 400.5 ) );
      m_pcPitchbendRange->setCurrentItem( (int)( ( ( sample()->getPitchbendRange() + 12.0 ) / 0.1 ) + 0.5 ) );
   }
}

