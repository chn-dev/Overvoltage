#ifndef __PLUGINEDITOR_H__
#define __PLUGINEDITOR_H__

#include <SamplerGUI/SamplerGUI.h>
#include <SamplerGUI/UIPage.h>
#include "PluginProcessor.h"

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

   void onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pSamplerKeyboard );

   virtual void buttonClicked( juce::Button *pButton );
   virtual void buttonStateChanged( juce::Button *pButton );

   size_t currentPart() const;

   PluginProcessor &processor() const;

private:
   void activatePart( size_t nPart );

   // This reference is provided as a quick way for your editor to
   // access the processor object that created it.
   PluginProcessor &processorRef;

   std::vector<SamplerGUI::UIPage *> m_UIPages;

   size_t m_CurrentPart;
   std::vector<juce::TextButton *> m_PartButtons;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( PluginEditor )
};

#endif

