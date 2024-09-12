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
\file UISectionFilter.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionFilter.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONFILTER_H__
#define __UISECTIONFILTER_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionFilter
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionFilter : public UISection,
                           public juce::Slider::Listener,
                           public juce::ComboBox::Listener
   {
   public:
      UISectionFilter( UIPage *pUIPage );
      ~UISectionFilter();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void sliderValueChanged( Slider *pSlider );
      virtual void comboBoxChanged( ComboBox *pComboBox );

   protected:
      void filterUpdated( SamplerEngine::Filter *pFilter );

   private:
      juce::Slider *m_psCutoff;
      juce::Label *m_plCutoff;

      juce::Slider *m_psResonance;
      juce::Label *m_plResonance;

      juce::ComboBox *m_pcbType;
   };
}

#endif

