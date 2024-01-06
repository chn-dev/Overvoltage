#pragma once

#include "JuceHeader.h"

#include "UISection.h"

class AudioPluginAudioProcessorEditor;

//==============================================================================
class SampleUISection : public UISection,
                        public juce::Slider::Listener,
                        public juce::Button::Listener,
                        public juce::ComboBox::Listener
{
public:
   SampleUISection( AudioPluginAudioProcessorEditor *pEditor );
   ~SampleUISection();

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
