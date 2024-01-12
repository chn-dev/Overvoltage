#pragma once

#include "JuceHeader.h"

#include "UISection.h"

class AudioPluginAudioProcessorEditor;

//==============================================================================
class NameRangesUISection : public UISection,
                            public juce::Label::Listener
{
public:
   NameRangesUISection( AudioPluginAudioProcessorEditor *pEditor );
   ~NameRangesUISection();

   virtual void paint( juce::Graphics &g );
   virtual void resized();

   virtual void samplesUpdated();

   virtual void labelTextChanged( Label *pLabel );

protected:

private:
   juce::Label *m_plLow;
   CycleComponent *m_pcMinNote;
   juce::Label *m_plHigh;
   CycleComponent *m_pcMaxNote;

   CycleComponent *m_pcMinVelocity;
   CycleComponent *m_pcMaxVelocity;

   juce::Label *m_plBaseNote;
   juce::Label *m_plK1;
   juce::Label *m_plK2;
   juce::Label *m_plV1;
   juce::Label *m_plV2;

   juce::Label *m_plName;

   CycleComponent *m_pcBaseNote;
};
