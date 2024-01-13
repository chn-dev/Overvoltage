#ifndef __PLUGINPROCESSOR_H__
#define __PLUGINPROCESSOR_H__

#include <map>
#include <list>

#include <juce_audio_processors/juce_audio_processors.h>

#include <SamplerEngine/Voice.h>
#include <SamplerEngine/Part.h>

#include "SamplerKeyboard.h"

//==============================================================================
class PluginProcessor : public juce::AudioProcessor,
                        public juce::MidiKeyboardStateListener,
                        public juce::MidiKeyboardState,
                        public SamplerKeyboardListener
{
public:
   //==============================================================================
   PluginProcessor();
   ~PluginProcessor() override;

   virtual void onDeleteSample( size_t part, Overvoltage::Sample *pSample );

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
   void changeProgramName( int index, const juce::String& newName ) override;

   //==============================================================================
   void getStateInformation( juce::MemoryBlock& destData ) override;
   void setStateInformation( const void* data, int sizeInBytes ) override;

   virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
   virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

   std::list<Overvoltage::Sample *> &samples();
   const std::list<Overvoltage::Sample *> &constSamples() const;
   Overvoltage::SamplerEngine *samplerEngine() const;

private:
   bool outputBusReady( juce::AudioBuffer<float>& buffer, int n ) const;

   //==============================================================================
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( PluginProcessor )

   Overvoltage::SamplerEngine *m_pEngine;
   PluginEditor *m_pEditor;

   double m_sampleRate;
   int m_samplesPerBlock;
};

#endif
