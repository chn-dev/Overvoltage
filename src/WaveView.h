#pragma once

#include "JuceHeader.h"

#include "UISection.h"
#include "Sample.h"
#include "SamplerKeyboard.h"

class AudioPluginAudioProcessorEditor;

//==============================================================================
class WaveView : public UISection
{
public:
   WaveView( AudioPluginAudioProcessorEditor *pEditor );
   ~WaveView();

   void paint( juce::Graphics & ) override;
   void resized() override;

   virtual void samplesUpdated();

   virtual void mouseDown( const MouseEvent &event );
   virtual void mouseDrag( const MouseEvent &event );
   virtual void mouseUp( const MouseEvent & event );
   virtual void mouseMove( const MouseEvent &event );

protected:
   uint32_t m_OrigLoopPoint;
   bool m_IsDraggingLoopStart;
   bool m_IsDraggingLoopEnd;

private:
};
