/*----------------------------------------------------------------------------*/
/*!
\file UISectionLFO.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionLFO.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONLFO_H__
#define __UISECTIONLFO_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionLFO
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionLFO : public UISection,
                        public juce::Button::Listener,
                        public juce::ComboBox::Listener,
                        public juce::Label::Listener
   {
   public:
      class StepEditor : public juce::Component,
                         public juce::Label::Listener,
                         public juce::Button::Listener
      {
         public:
            StepEditor( UISectionLFO *pSectionLFO );
            ~StepEditor();

            virtual void paint( juce::Graphics &g ) override;

            void mouseMove( const MouseEvent &event) override;
            void mouseDrag( const MouseEvent &event ) override;
            void mouseDown( const MouseEvent &event ) override;
            void mouseUp( const MouseEvent &event ) override;

            virtual void labelTextChanged( Label *pLabel );
            virtual void buttonClicked( Button *pButton );
            virtual void buttonStateChanged( Button *pButton );

            void update();

         private:
            void changeStepValue( int x, int y );
            int getDiagramXPos() const;
            int getDiagramYPos() const;
            int getDiagramWidth() const;
            int getDiagramHeight() const;

         private:
            UISectionLFO *m_pSectionLFO;

            juce::Label *m_plNumSteps;
            CycleComponent *m_pcNumSteps;

            juce::TextButton *m_pbQuantize;
            CycleComponent *m_pcQuantize;
      };

      UISectionLFO( UIPage *pUIPage, std::string label );
      ~UISectionLFO();

      void setCurrentLFO( size_t n );
      size_t getCurrentLFO() const;

      SamplerEngine::LFO *getLFO() const;

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
      juce::Label *m_plWaveform;
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
