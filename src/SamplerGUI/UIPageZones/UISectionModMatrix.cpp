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
\file UISectionModMatrix.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the modulation matrix UI section
*/
/*----------------------------------------------------------------------------*/
#include "UISectionModMatrix.h"

#include <util.h>

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionModMatrix::UIModSlot::UIModSlot() :
   m_pSectionModMatrix( nullptr ),
   m_pcbSrc( nullptr ),
   m_pcbMod( nullptr ),
   m_pcbDest( nullptr ),
   m_pbEnabled( nullptr ),
   m_pccAmount( nullptr ),
   m_pcbMathFunc( nullptr ),
   m_pModSlot( nullptr ),
   m_Index( -1 ),
   m_Xp( -1 ),
   m_Yp( -1 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionModMatrix::UIModSlot::UIModSlot( UISectionModMatrix *pSectionModMatrix, SamplerEngine::ModMatrix::ModSlot *pModSlot, int index, int xp, int yp ) :
   m_pSectionModMatrix( pSectionModMatrix ),
   m_pModSlot( pModSlot ),
   m_Index( index ),
   m_Xp( xp ),
   m_Yp( yp )
{
   std::vector<juce::Component *> c;

   m_pbEnabled = new juce::TextButton( stdformat( "{}", index ) );
   m_pbEnabled->setToggleable( true );
   m_pbEnabled->setClickingTogglesState( true );
   m_pbEnabled->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbEnabled->setToggleState( pModSlot->isEnabled(), dontSendNotification );
   m_pbEnabled->addListener( this );
   c.push_back( m_pbEnabled );
   m_pbEnabled->setBounds( xp, yp, 18, 18 );

   m_pcbSrc = new juce::ComboBox( "Source" );
   for( SamplerEngine::ModMatrix::ModSrc modSrc : SamplerEngine::ModMatrix::allModSrc() )
   {
      m_pcbSrc->addItem( SamplerEngine::ModMatrix::toString( modSrc ), modSrc );
   }
   m_pcbSrc->setSelectedId( pModSlot->getSrc() );
   m_pcbSrc->addListener( this );
   c.push_back( m_pcbSrc );
   m_pcbSrc->setBounds( xp, yp, 120, 18 );

   m_pcbMod = new juce::ComboBox( "Modificator" );
   for( SamplerEngine::ModMatrix::ModSrc modSrc : SamplerEngine::ModMatrix::allModSrc() )
   {
      m_pcbMod->addItem( SamplerEngine::ModMatrix::toString( modSrc ), modSrc );
   }
   m_pcbMod->setSelectedId( pModSlot->getMod() );
   m_pcbMod->addListener( this );
   c.push_back( m_pcbMod );
   m_pcbMod->setBounds( xp, yp, 120, 18 );

   m_pccAmount = new CycleComponent();
   m_pccAmount->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pccAmount->addListener( this );
   m_pccAmount->setJustificationType( juce::Justification::centred );
   SamplerEngine::ModMatrix::ModDestInfo modDestInfo = pModSlot->getDest();
   m_pccAmount->setItems(
      modDestInfo.getMin(),
      modDestInfo.getMax(),
      modDestInfo.getStep(),
      modDestInfo.getFormat(),
      modDestInfo.getUnit() );
   int nAmount = (int)( ( ( pModSlot->getAmount() - modDestInfo.getMin() ) / modDestInfo.getStep() ) + 0.5 );
   m_pccAmount->setCurrentItem( nAmount );
   c.push_back( m_pccAmount );
   m_pccAmount->setBounds( xp, yp, 94, 18 );

   m_pcbDest = new juce::ComboBox( "Destination" );
   for( SamplerEngine::ModMatrix::ModDest modDest : SamplerEngine::ModMatrix::allModDest() )
   {
      m_pcbDest->addItem( SamplerEngine::ModMatrix::toString( modDest ), modDest );
   }
   m_pcbDest->setSelectedId( pModSlot->getDest() );
   m_pcbDest->addListener( this );
   c.push_back( m_pcbDest );
   m_pcbDest->setBounds( xp, yp, 120, 18);

   m_pcbMathFunc = new juce::ComboBox( "MathFunc" );
   for( SamplerEngine::ModMatrix::MathFunc mathFunc : SamplerEngine::ModMatrix::allMathFunc() )
   {
      m_pcbMathFunc->addItem( SamplerEngine::ModMatrix::toString( mathFunc ), mathFunc );
   }
   m_pcbMathFunc->setSelectedId( pModSlot->getMathFunc() );
   m_pcbMathFunc->addListener( this );
   c.push_back( m_pcbMathFunc );
   m_pcbMathFunc->setBounds( xp, yp, 72, 18 );

   for( size_t i = 0; i < c.size(); i++ )
   {
      juce::Component *pC = c[i];
      if( i == 0 )
      {
         pC->setBounds( xp, yp, pC->getWidth(), pC->getHeight() );
      } else
      {
         pC->setBounds( c[i - 1]->getX() + c[i - 1]->getWidth() + 4, yp, pC->getWidth(), pC->getHeight() );
         pC->setEnabled( m_pModSlot->isEnabled() );
      }
      pSectionModMatrix->addAndMakeVisible( pC );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionModMatrix::UIModSlot::~UIModSlot()
{
   if( m_pbEnabled )
   {
      delete m_pbEnabled;
   }

   if( m_pccAmount )
   {
      delete m_pccAmount;
   }

   if( m_pcbSrc )
   {
      delete m_pcbSrc;
   }

   if( m_pcbMod )
   {
      delete m_pcbMod;
   }

   if( m_pcbDest )
   {
      delete m_pcbDest;
   }

   if( m_pcbMathFunc )
   {
      delete m_pcbMathFunc;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The modulation slot
*/
/*----------------------------------------------------------------------------*/
SamplerEngine::ModMatrix::ModSlot *UISectionModMatrix::UIModSlot::getModSlot() const
{
   return( m_pModSlot );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
\param pButton The button that has been clicked
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::UIModSlot::buttonClicked( Button *pButton )
{
   if( m_pbEnabled == pButton )
   {
      m_pModSlot->setEnabled( m_pbEnabled->getToggleState() );
      m_pcbSrc->setEnabled( m_pModSlot->isEnabled() );
      m_pcbMod->setEnabled( m_pModSlot->isEnabled() );
      m_pcbDest->setEnabled( m_pModSlot->isEnabled() );
      m_pccAmount->setEnabled( m_pModSlot->isEnabled() );
      m_pcbMathFunc->setEnabled( m_pModSlot->isEnabled() );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Button::Listener
\param pButton The button whose state has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::UIModSlot::buttonStateChanged( Button */*pButton*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Label::Listener
\param pLabel The label that has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::UIModSlot::labelTextChanged( Label *pLabel )
{
   if( m_pccAmount == pLabel )
   {
      SamplerEngine::ModMatrix::ModDestInfo modDestInfo = m_pModSlot->getDest();
      m_pModSlot->setAmount( ( m_pccAmount->getCurrentItem() * modDestInfo.getStep() ) + modDestInfo.getMin() );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::ComboBox::Listener
\param pComboBox The ComboBox that has changed
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::UIModSlot::comboBoxChanged( ComboBox *pComboBox )
{
   if( pComboBox == m_pcbSrc )
   {
      SamplerEngine::ModMatrix::ModSrc selectedModSrc = (SamplerEngine::ModMatrix::ModSrc)m_pcbSrc->getSelectedId();
      if( selectedModSrc != m_pModSlot->getSrc() )
      {
         m_pModSlot->setSrc( selectedModSrc );
      }
   } else
   if( pComboBox == m_pcbMod )
   {
      SamplerEngine::ModMatrix::ModSrc mod = (SamplerEngine::ModMatrix::ModSrc)m_pcbMod->getSelectedId();
      if( mod != m_pModSlot->getMod() )
      {
         m_pModSlot->setMod( mod );
      }
   } else
   if( pComboBox == m_pcbMathFunc )
   {
      SamplerEngine::ModMatrix::MathFunc mathFunc = (SamplerEngine::ModMatrix::MathFunc)m_pcbMathFunc->getSelectedId();
      if( mathFunc != m_pModSlot->getMathFunc() )
      {
         m_pModSlot->setMathFunc( mathFunc );
      }
   } else
   if( pComboBox == m_pcbDest )
   {
      SamplerEngine::ModMatrix::ModDest selectedModDest = (SamplerEngine::ModMatrix::ModDest)m_pcbDest->getSelectedId();
      if( selectedModDest != m_pModSlot->getDest() )
      {
         SamplerEngine::ModMatrix::ModDestInfo modDestInfo = selectedModDest;
         m_pModSlot->setDest( selectedModDest );
         m_pModSlot->setAmount( modDestInfo.getDefaultValue() );
         m_pccAmount->setItems(
            modDestInfo.getMin(),
            modDestInfo.getMax(),
            modDestInfo.getStep(),
            modDestInfo.getFormat(),
            modDestInfo.getUnit() );
         int nAmount = (int)( ( ( m_pModSlot->getAmount() - modDestInfo.getMin() ) / modDestInfo.getStep() ) + 0.5 );
         m_pccAmount->setCurrentItem( nAmount );
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionModMatrix::UISectionModMatrix( UIPage *pUIPage ) :
   UISection( pUIPage, "Modulation Matrix" )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionModMatrix::~UISectionModMatrix()
{
   for( size_t i = 0; i < m_UIModSlots.size(); i++ )
   {
      delete m_UIModSlots[i];
   }
   m_UIModSlots.clear();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::resized()
{
   UISection::resized();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the user has (de-)selected any samples
*/
/*----------------------------------------------------------------------------*/
void UISectionModMatrix::samplesUpdated()
{
   const SamplerEngine::Sample *pSample = sample();

   for( size_t i = 0; i < m_UIModSlots.size(); i++ )
   {
      delete m_UIModSlots[i];
   }
   m_UIModSlots.clear();

   if( pSample )
   {
      for( size_t i = 0; i < pSample->getModMatrix()->numSlots(); i++ )
      {
         UIModSlot *pUIModSlot = new UIModSlot( this, pSample->getModMatrix()->getSlot( i ), (int)i, 6, 28 + ( 18 * (int)i ) );
         m_UIModSlots.push_back( pUIModSlot );
      }
   }
}

