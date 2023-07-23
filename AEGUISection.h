#pragma once

#include "JuceHeader.h"

#include "EGUISection.h"

class AudioPluginAudioProcessorEditor;

//==============================================================================
class AEGUISection : public EGUISection
{
public:
   AEGUISection( AudioPluginAudioProcessorEditor *pEditor );
   ~AEGUISection();

   virtual void paint( juce::Graphics &g );
   virtual void resized();
   
   virtual void sampleUpdated();

protected:

private:
};
