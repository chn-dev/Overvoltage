/*----------------------------------------------------------------------------*/
/*!
\file UISectionOutput.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionOutput.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONOUTPUT_H__
#define __UISECTIONOUTPUT_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionOutput
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionOutput : public UISection,
                           public juce::Label::Listener,
                           public juce::Slider::Listener,
                           public juce::ComboBox::Listener
   {
   public:
      UISectionOutput( UIPage *pUIPage );
      ~UISectionOutput();

      virtual void paint( juce::Graphics &g );
      virtual void resized();
      virtual void samplesUpdated();
      virtual void labelTextChanged( Label *pLabel );
      virtual void sliderValueChanged( Slider *pSlider );
      virtual void comboBoxChanged( ComboBox *pComboBox );

   protected:

   private:
      juce::ComboBox *m_pcbOutputBus;
      juce::Slider *m_psPan;
      juce::Label *m_plPan;
      juce::Slider *m_psGain;
      juce::Label *m_plGain;
   };
}
#endif
