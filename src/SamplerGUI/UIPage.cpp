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
\file UIPage.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a UI page
*/
/*----------------------------------------------------------------------------*/
#include "UIPage.h"
#include "UIPageZones/UISectionSamplerKeyboard.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UIPage::UIPage( PluginEditor *pEditor ) :
   m_pEditor( pEditor )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UIPage::~UIPage()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UIPage::paint( juce::Graphics &/*g*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UIPage::resized()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSection
*/
/*----------------------------------------------------------------------------*/
void UIPage::addUISection( SamplerGUI::UISection *pSection )
{
   if( pSection )
   {
      m_UISections.insert( pSection );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSection
*/
/*----------------------------------------------------------------------------*/
void UIPage::removeUISection( SamplerGUI::UISection *pSection )
{
   if( pSection && ( m_UISections.find( pSection ) != m_UISections.end() ) )
   {
      m_UISections.erase( pSection );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
This is called when the user has (de-)selected samples.
\param pKeyboard The sampler keyboard
*/
/*----------------------------------------------------------------------------*/
void UIPage::onSampleSelectionUpdated( UISectionSamplerKeyboard *pKeyboard )
{
   for( UISection *pSection : m_UISections )
   {
      pSection->onSampleSelectionUpdated( pKeyboard );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Update all UI sections
*/
/*----------------------------------------------------------------------------*/
void UIPage::updateUISections()
{
   for( SamplerGUI::UISection *pSection : m_UISections )
   {
      pSection->samplesUpdated();
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The plugin editor
*/
/*----------------------------------------------------------------------------*/
PluginEditor *UIPage::editor() const
{
   return( m_pEditor );
}

