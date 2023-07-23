#pragma once

#include <map>
#include <list>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Keyboard.h"
#include "Voice.h"

//==============================================================================
class AudioPluginAudioProcessor  : public juce::AudioProcessor,
                                   public juce::MidiKeyboardStateListener,
                                   public juce::MidiKeyboardState
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
    virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

    std::list<Sample *> &samples();
    const std::list<Sample *> &constSamples() const;
    bool midiNoteIsPlaying( int midiNote ) const;
    bool isPlaying( const Sample *pSample ) const;
    std::set<int> allPlayingMidiNotes() const;
    void stopVoice( const Voice *pVoice );

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
    AudioPluginAudioProcessorEditor *m_pEditor;
    std::list<Sample *> getSamplesByMidiNoteAndVelocity( int note, int vel ) const;

    std::multimap<int, Voice *> m_Voices;
    std::list<Sample *> m_Samples;

    double m_sampleRate;
    int m_samplesPerBlock;
    int m_ofs;
    int m_note;
};
