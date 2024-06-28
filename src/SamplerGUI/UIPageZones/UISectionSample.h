/*----------------------------------------------------------------------------*/
/*!
\file UISectionSample.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionSample.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONSAMPLE_H__
#define __UISECTIONSAMPLE_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionSample
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionSample : public UISection,
                           public juce::Slider::Listener,
                           public juce::Button::Listener,
                           public juce::ComboBox::Listener
   {
   public:
      UISectionSample( UIPage *pUIPage );
      ~UISectionSample();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void sliderValueChanged( Slider *pSlider );

      virtual void buttonClicked( Button *pButton );
      virtual void buttonStateChanged( Button *pButton );

      virtual void comboBoxChanged( ComboBox *pComboBox );

   protected:

   private:
      juce::Label *m_plDetune;
      juce::Slider *m_psDetune;

      juce::ComboBox *m_pcbPlayMode;
      juce::TextButton *m_pbReverse;
   };
}
#endif
