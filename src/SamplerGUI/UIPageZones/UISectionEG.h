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
\file UISectionEG.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionEG.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONEG_H__
#define __UISECTIONEG_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionEG
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionEG : public UISection,
                       public juce::Slider::Listener
   {
   public:
      UISectionEG( UIPage *pUIPage, std::string label );
      ~UISectionEG();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void sliderValueChanged( Slider *pSlider );

      void egUpdated( SamplerEngine::ENV *pENV );

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const = 0;

   private:
      juce::Slider *m_psAttack;
      juce::Label *m_plAttack;

      juce::Slider *m_psDecay;
      juce::Label *m_plDecay;

      juce::Slider *m_psSustain;
      juce::Label *m_plSustain;

      juce::Slider *m_psRelease;
      juce::Label *m_plRelease;
   };
}

#endif

