#ifndef __UISECTIONFILTER_H__
#define __UISECTIONFILTER_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
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
