#pragma once

#include "JuceHeader.h"

#include "UISection.h"

class AudioPluginAudioProcessorEditor;

//==============================================================================
class OutputUISection : public UISection,
                        public juce::Label::Listener,
                        public juce::Slider::Listener,
                        public juce::ComboBox::Listener
{
public:
   OutputUISection( AudioPluginAudioProcessorEditor *pEditor );
   ~OutputUISection();

   virtual void paint( juce::Graphics &g );
   virtual void resized();
   virtual void sampleUpdated();
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
