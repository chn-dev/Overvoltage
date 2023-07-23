#pragma once

#include "PluginProcessor.h"

#include "SamplerKeyboard.h"
#include "WaveView.h"
#include "SampleUISection.h"
#include "NameRangesUISection.h"
#include "AEGUISection.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::Button::Listener,
                                         public juce::MidiKeyboardStateListener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
    virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

    void addUISection( UISection *pSection );
    void removeUISection( UISection *pSection );

    void onSampleSelectionUpdated( SamplerKeyboard *pSamplerKeyboard );

    AudioPluginAudioProcessor &processor() const;

private:
   void buttonClicked( juce::Button *pButton );

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    juce::TextButton *m_pButton;

    std::set<UISection *> m_UISections;

    SamplerKeyboard *m_pKeyboard;
    WaveView *m_pWaveView;
    SampleUISection *m_pSampleSection;
    NameRangesUISection *m_pNameRangesUISection;
    AEGUISection *m_pAEGUISection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
