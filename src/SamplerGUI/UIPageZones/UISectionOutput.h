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
\file UISectionOutput.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionOutput.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONOUTPUT_H__
#define __UISECTIONOUTPUT_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionOutput
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionOutput : public UISection,
                           public juce::Label::Listener,
                           public juce::Slider::Listener,
                           public juce::ComboBox::Listener
   {
   public:
      UISectionOutput( UIPage *pUIPage );
      ~UISectionOutput();

      virtual void paint( juce::Graphics &g );
      virtual void resized();
      virtual void samplesUpdated();
      virtual void labelTextChanged( Label *pLabel );
      virtual void sliderValueChanged( Slider *pSlider );
      virtual void comboBoxChanged( ComboBox *pComboBox );

   protected:

   private:
      juce::ComboBox *m_pcbOutputBus;
      juce::Slider *m_psPan;
      juce::Label *m_plPan;
      juce::Slider *m_psGain;
      juce::Label *m_plGain;
   };
}

#endif

