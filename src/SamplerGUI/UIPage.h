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
\file UIPage.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UIPage.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UIPAGE_H__
#define __UIPAGE_H__

#include "JuceHeader.h"

class PluginEditor;

namespace SamplerGUI
{
   class UISection;
   class UISectionSamplerKeyboard;

   /*----------------------------------------------------------------------------*/
   /*!
   \class UIPage
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UIPage : public juce::Component
   {
   public:
      UIPage( PluginEditor *pEditor );
      ~UIPage();

      virtual void currentPartChanged( size_t nPart ) = 0;
      void onSampleSelectionUpdated( UISectionSamplerKeyboard *pKeyboard );

      virtual void paint( juce::Graphics &g ) override;
      virtual void resized() override;

      PluginEditor *editor() const;

      void addUISection( SamplerGUI::UISection *pSection );
      void removeUISection( SamplerGUI::UISection *pSection );

   protected:
      void updateUISections();

   protected:
      PluginEditor *m_pEditor;

   private:
      std::set<SamplerGUI::UISection *> m_UISections;
   };
}

#endif

