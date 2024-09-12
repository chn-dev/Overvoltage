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
\file UIPageZones.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UIPageZones.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UIPAGEZONES_H__
#define __UIPAGEZONES_H__

#include <SamplerGUI/UIPage.h>

#include "UISectionWaveView.h"
#include "UISectionSample.h"
#include "UISectionNameRanges.h"
#include "UISectionAEG.h"
#include "UISectionEG2.h"
#include "UISectionLFO.h"
#include "UISectionFilter.h"
#include "UISectionOutput.h"
#include "UISectionModMatrix.h"
#include "UISectionSamplerKeyboard.h"

class PluginEditor;

namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UIPageZones
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UIPageZones : public UIPage,
                       public juce::Button::Listener
   {
   public:
      UIPageZones( PluginEditor *pEditor );
      ~UIPageZones();

      virtual void currentPartChanged( size_t nPart );
      virtual void buttonClicked( Button *pButton );
      virtual void buttonStateChanged( Button *pButton );

      int getCurrentLayer() const;
      void setCurrentLayer( int nLayer );

      SamplerGUI::UISectionSamplerKeyboard *getSamplerKeyboard() const;

      bool isSoloEnabled() const;

   protected:
      SamplerGUI::UISectionSamplerKeyboard *m_pUISectionKeyboard;
      SamplerGUI::UISectionWaveView *m_pUISectionWaveView;
      SamplerGUI::UISectionSample *m_pUISectionSample;
      SamplerGUI::UISectionNameRanges *m_pUISectionNameRanges;
      SamplerGUI::UISectionAEG *m_pUISectionAEG;
      SamplerGUI::UISectionEG2 *m_pUISectionEG2;
      SamplerGUI::UISectionLFO *m_pUISectionLFO;
      SamplerGUI::UISectionFilter *m_pUISectionFilter;
      SamplerGUI::UISectionOutput *m_pUISectionOutput;
      SamplerGUI::UISectionModMatrix *m_pUISectionModMatrix;

      std::vector<juce::TextButton *> m_LayerButtons;
      juce::TextButton *m_pbSolo;
      juce::TextButton *m_pbSelLayer;
      juce::TextButton *m_pbSelAll;
   };
}

#endif

