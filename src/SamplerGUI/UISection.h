#ifndef __UISECTION_H__
#define __UISECTION_H__

#include "JuceHeader.h"
#include "SamplerKeyboard.h"
#include "UIPage.h"

class PluginEditor;


//==============================================================================
namespace SamplerGUI
{
   class UIPage;

   class UISection : public juce::Component
   {
   public:
      UISection( UIPage *pUIPage, std::string label = std::string() );
      ~UISection();

      virtual void paint( juce::Graphics &g ) override;
      virtual void resized() override;

      void onSampleSelectionUpdated( SamplerKeyboard *pSamplerKeyboard );

      virtual void samplesUpdated() = 0;

   protected:
      class SliderLookAndFeel : public juce::LookAndFeel_V4
      {
      public:
         virtual void drawLinearSlider(
            Graphics &g,
            int x, int y, int width, int height,
            float sliderPos, float minSliderPos, float maxSliderPos,
            const Slider::SliderStyle sliderStyle, Slider &slider );
      };
      SliderLookAndFeel m_SliderLAF;

      class CycleComponent : public juce::Label
      {
      public:
         CycleComponent( std::vector<std::string> items );
         ~CycleComponent();
         void setCurrentItem( int item );
         int getCurrentItem() const;

         virtual void mouseDown( const MouseEvent &event );
         virtual void mouseDrag( const MouseEvent &event );
         virtual void mouseUp( const MouseEvent &event );

      private:
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
