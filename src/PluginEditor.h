#ifndef __PLUGINEDITOR_H__
#define __PLUGINEDITOR_H__

#include "PluginProcessor.h"

#include "SamplerKeyboard.h"
#include "WaveView.h"
#include "SampleUISection.h"
#include "NameRangesUISection.h"
#include "AEGUISection.h"
#include "OutputUISection.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor,
                     public juce::Button::Listener,
                     public juce::MidiKeyboardStateListener
{
public:
   explicit PluginEditor( PluginProcessor & );
   ~PluginEditor() override;

   //==============================================================================
   void paint( juce::Graphics& ) override;
   void resized() override;

   virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
   virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

   void addUISection( UISection *pSection );
   void removeUISection( UISection *pSection );

   void onSampleSelectionUpdated( SamplerKeyboard *pSamplerKeyboard );

   virtual void buttonClicked( juce::Button *pButton );
   virtual void buttonStateChanged( juce::Button *pButton );

   size_t currentPart() const;

   PluginProcessor &processor() const;

private:
   void activatePart( size_t nPart );

   // This reference is provided as a quick way for your editor to
   // access the processor object that created it.
   PluginProcessor &processorRef;

   std::set<UISection *> m_UISections;

   SamplerKeyboard *m_pKeyboard;
   WaveView *m_pWaveView;
   SampleUISection *m_pSampleSection;
   NameRangesUISection *m_pNameRangesUISection;
   AEGUISection *m_pAEGUISection;
   OutputUISection *m_pOutputUISection;

   size_t m_CurrentPart;
   std::vector<juce::TextButton *> m_PartButtons;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( PluginEditor )
};

#endif

