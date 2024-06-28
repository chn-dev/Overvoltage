/*----------------------------------------------------------------------------*/
/*!
\file PluginProcessor.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class PluginProcessor.
*/
/*----------------------------------------------------------------------------*/
#ifndef __PLUGINPROCESSOR_H__
#define __PLUGINPROCESSOR_H__

#include <map>
#include <list>

#include <juce_audio_processors/juce_audio_processors.h>

#include <SamplerEngine/Voice.h>
#include <SamplerEngine/Part.h>

#include <SamplerGUI/UIPageZones/UISectionSamplerKeyboard.h>

/*----------------------------------------------------------------------------*/
/*!
\class PluginProcessor
\date  2024-06-10
*/
/*----------------------------------------------------------------------------*/
class PluginProcessor : public juce::AudioProcessor,
                        public juce::MidiKeyboardStateListener,
                        public juce::MidiKeyboardState,
                        public SamplerGUI::UISectionSamplerKeyboardListener
{
public:
   //==============================================================================
   PluginProcessor();
   ~PluginProcessor() override;

   virtual void onDeleteSample( size_t part, SamplerEngine::Sample *pSample );
   virtual void onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pKeyboard );

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

   void handlePitchbend( int midiChannel, double v );
   void handleControllerChange( int midiChannel, int ccNum, double v );

   std::list<SamplerEngine::Sample *> &samples();
   const std::list<SamplerEngine::Sample *> &constSamples() const;
   void importMulti( juce::XmlElement *pXmlMulti );
   SamplerEngine::Engine *samplerEngine() const;
   PluginEditor *pluginEditor() const;

private:
   bool outputBusReady( juce::AudioBuffer<float>& buffer, int n ) const;

   //==============================================================================
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( PluginProcessor )

   SamplerEngine::Engine *m_pEngine;
   PluginEditor *m_pEditor;

   double m_sampleRate;
   int m_samplesPerBlock;
};

#endif
