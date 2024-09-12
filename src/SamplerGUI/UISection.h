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
\file UISection.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISection.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTION_H__
#define __UISECTION_H__

#include "JuceHeader.h"
#include <SamplerEngine/SamplerEngine.h>

class PluginEditor;


//==============================================================================
namespace SamplerGUI
{
   class UIPage;
   class UISectionSamplerKeyboard;

   /* ----------------------------------------------------------------------------*/
  /*!
   \class UISection
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISection : public juce::Component
   {
   public:
      class SliderLookAndFeel : public juce::LookAndFeel_V4
      {
      public:
         virtual void drawLinearSlider(
            Graphics &g,
            int x, int y, int width, int height,
            float sliderPos, float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle sliderStyle, Slider &slider );
      };

      UISection( UIPage *pUIPage, std::string label = std::string() );
      ~UISection();

      virtual void paint( juce::Graphics &g ) override;
      virtual void resized() override;

      void onSampleSelectionUpdated( UISectionSamplerKeyboard *pSamplerKeyboard );

      virtual void samplesUpdated() = 0;

   protected:
      SliderLookAndFeel m_SliderLAF;

      class CycleComponent : public juce::Label
      {
      public:
         CycleComponent( std::vector<std::string> items );
         CycleComponent();
         ~CycleComponent();
         void setCurrentItem( int item );
         int getCurrentItem() const;

         void setCurrentItemByValue( double value );
         double getCurrentItemValue() const;

         void setItems( double minVal, double maxVal, double step, std::string format, std::string unit );
         double getMinVal() const;
         double getMaxVal() const;
         double getStep() const;

         virtual void mouseDown( const MouseEvent &event );
         virtual void mouseDrag( const MouseEvent &event );
         virtual void mouseUp( const MouseEvent &event );

      private:
         double m_MinVal;
         double m_MaxVal;
         double m_Step;
         std::vector<std::string> m_Items;
         int m_CurrentItem;
         juce::Point<float> m_MouseStartPoint;
         int m_PixelsPerItem;
         int m_DY;
         juce::MouseCursor m_OrigMouseCursor;
         juce::Colour m_OrigColour;
      };

   protected:
      const std::set<SamplerEngine::Sample *> &samples() const;
      SamplerEngine::Sample *sample() const;
      UIPage *uiPage() const;

   private:
      UIPage *m_pUIPage;
      juce::Label *m_pLabel;
      std::set<SamplerEngine::Sample *> m_Samples;
   };
}
#endif

