/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of chn's Overvoltage.                                   *
 *                                                                             *
 *  Overvoltage is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *          
 *  Overvoltage is distributed in the hope that it will be useful, but         * 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Overvoltage. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file PluginEditor.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class PluginEditor.
*/
/*----------------------------------------------------------------------------*/
#ifndef __PLUGINEDITOR_H__
#define __PLUGINEDITOR_H__

#include <SamplerGUI/SamplerGUI.h>
#include "PluginProcessor.h"

/*----------------------------------------------------------------------------*/
/*!
\class PluginEditor
\date  2024-06-10
*/
/*----------------------------------------------------------------------------*/
class PluginEditor : public juce::AudioProcessorEditor,
                     public juce::Button::Listener,
                     public juce::MidiKeyboardStateListener
{
public:
   explicit PluginEditor( PluginProcessor & );
   ~PluginEditor() override;

   void paint( juce::Graphics& ) override;
   void resized() override;

   virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
   virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

   void onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pSamplerKeyboard );

   virtual void buttonClicked( juce::Button *pButton );
   virtual void buttonStateChanged( juce::Button *pButton );

   SamplerGUI::UIPageZones *getUIPageZones() const;

   std::set<SamplerEngine::Sample *> getSelectedSamples() const;
   bool isSoloEnabled() const;

   size_t currentPart() const;
   void activatePart( size_t nPart );

   PluginProcessor &processor() const;

private:

   // This reference is provided as a quick way for your editor to
   // access the processor object that created it.
   PluginProcessor &processorRef;

   std::vector<SamplerGUI::UIPage *> m_UIPages;

   size_t m_CurrentPart;
   std::vector<juce::TextButton *> m_PartButtons;
   juce::TextButton *m_pExportProgram;
   juce::TextButton *m_pImportProgram;
   juce::TextButton *m_pExportMulti;
   juce::TextButton *m_pImportMulti;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( PluginEditor )
};

#endif

