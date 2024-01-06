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

protected:
   virtual ENV *getENV( const Sample *pSample ) const;
};
