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
                        public juce::Label::Listener
   {
   public:
      class StepEditor : public juce::Component
      {
         public:
            StepEditor( UISectionLFO *pSectionLFO );
            ~StepEditor();

            virtual void paint( juce::Graphics &g ) override;

            void setSteps( std::vector<double> s );

         private:
            UISectionLFO *m_pSectionLFO;
            std::vector<double> m_Steps;
      };

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
      virtual void labelTextChanged( Label *pLabel );

   protected:
      void updateInfo();

   private:
      std::vector<juce::TextButton *> m_SelButtons;
      juce::ComboBox *m_pcbWaveform;

      juce::Label *m_plRate;
      CycleComponent *m_pcRate;
      juce::TextButton *m_pbRateSync;

      juce::Label *m_plDelay;
      CycleComponent *m_pcDelay;
      juce::TextButton *m_pbDelaySync;

      juce::Label *m_plFadeIn;
      CycleComponent *m_pcFadeIn;
      juce::TextButton *m_pbFadeInSync;

      juce::TextButton *m_pbOnce;
      juce::TextButton *m_pbRndPhase;

      StepEditor *m_pStepEditor;
   };
}
#endif
