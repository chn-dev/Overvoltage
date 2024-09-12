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
\file UISectionSample.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionSample.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONSAMPLE_H__
#define __UISECTIONSAMPLE_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionSample
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionSample : public UISection,
                           public juce::Slider::Listener,
                           public juce::Button::Listener,
                           public juce::ComboBox::Listener
   {
   public:
      UISectionSample( UIPage *pUIPage );
      ~UISectionSample();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void sliderValueChanged( Slider *pSlider );

      virtual void buttonClicked( Button *pButton );
      virtual void buttonStateChanged( Button *pButton );

      virtual void comboBoxChanged( ComboBox *pComboBox );

   protected:

   private:
      juce::Label *m_plDetune;
      juce::Slider *m_psDetune;

      juce::ComboBox *m_pcbPlayMode;
      juce::TextButton *m_pbReverse;
   };
}

#endif

