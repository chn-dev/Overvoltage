#ifndef __UISECTIONLFO_H__
#define __UISECTIONLFO_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class UISectionLFO : public UISection,
                        public juce::Button::Listener,
                        public juce::ComboBox::Listener,
                        public juce::Slider::Listener
   {
   public:
      UISectionLFO( UIPage *pUIPage, std::string label );
      ~UISectionLFO();

      void setCurrentLFO( size_t n );
      size_t getCurrentLFO() const;

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void buttonClicked( Button *pButton );
      virtual void buttonStateChanged( Button *pButton );

      virtual void comboBoxChanged( ComboBox *pComboBox );

      virtual void sliderValueChanged( Slider *pSlider );

   protected:

   private:
      std::vector<juce::TextButton *> m_SelButtons;
      juce::ComboBox *m_pcbWaveform;
      juce::Label *m_plWaveform;

      juce::Label *m_plFrequency;
      juce::Slider *m_psFrequency;

      juce::TextButton *m_pbSync;
   };
}
#endif
