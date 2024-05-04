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
