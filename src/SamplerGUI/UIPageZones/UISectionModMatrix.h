#ifndef __UISECTIONMODMATRIX_H__
#define __UISECTIONMODMATRIX_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class UISectionModMatrix : public UISection
   {
   public:
      class UIModSlot : public juce::Slider::Listener,
                        public juce::ComboBox::Listener,
                        public juce::Button::Listener

      {
      public:
         class UISlider : public juce::Slider
         {
         public:
            UISlider( const String &name, UIModSlot *pModSlot );

         private:
            virtual String getTextFromValue( double value );

         private:
            UIModSlot *m_pModSlot;
         };

         UIModSlot();
         UIModSlot( UISectionModMatrix *pSectionModMatrix, SamplerEngine::ModMatrix::ModSlot *pModSlot, int index, int xp, int yp );
         ~UIModSlot();

         SamplerEngine::ModMatrix::ModSlot *getModSlot() const;
         virtual void sliderValueChanged( Slider *pSlider );
         virtual void comboBoxChanged( ComboBox *pComboBox );
         virtual void buttonClicked( Button *pButton );
         virtual void buttonStateChanged( Button *pButton );

      private:
         SliderLookAndFeel m_SliderLAF;
         UISectionModMatrix *m_pSectionModMatrix;
         juce::ComboBox *m_pcbSrc;
         juce::ComboBox *m_pcbDest;
         juce::TextButton *m_pbEnabled;
         juce::Slider *m_psAmount;
         SamplerEngine::ModMatrix::ModSlot *m_pModSlot;
         int m_Index;
         int m_Xp;
         int m_Yp;
      };

      UISectionModMatrix( UIPage *pUIPage );
      ~UISectionModMatrix();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

   protected:
      std::vector<UIModSlot *> m_UIModSlots;
   };
}
#endif
