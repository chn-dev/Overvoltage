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
\file UISectionNameRanges.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionNameRanges.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONNAMERANGES_H__
#define __UISECTIONNAMERANGES_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class AudioPluginAudioProcessorEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionNameRanges
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionNameRanges : public UISection,
                               public juce::Label::Listener
   {
   public:
      UISectionNameRanges( UIPage *pUIPage );
      ~UISectionNameRanges();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void labelTextChanged( Label *pLabel );

   protected:

   private:
      juce::Label *m_plLow;
      CycleComponent *m_pcMinNote;
      juce::Label *m_plHigh;
      CycleComponent *m_pcMaxNote;

      CycleComponent *m_pcMinVelocity;
      CycleComponent *m_pcMaxVelocity;

      juce::Label *m_plBaseNote;
      juce::Label *m_plK1;
      juce::Label *m_plK2;
      juce::Label *m_plV1;
      juce::Label *m_plV2;

      juce::Label *m_plKeytrack;
      CycleComponent *m_pcKeytrack;

      juce::Label *m_plPitchbendRange;
      CycleComponent *m_pcPitchbendRange;

      juce::Label *m_plName;

      CycleComponent *m_pcBaseNote;
   };
}

#endif

