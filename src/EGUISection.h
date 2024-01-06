#pragma once

#include "JuceHeader.h"

#include "UISection.h"

class AudioPluginAudioProcessorEditor;

//==============================================================================
class EGUISection : public UISection,
                     public juce::Slider::Listener
{
public:
   EGUISection( AudioPluginAudioProcessorEditor *pEditor, std::string label );
   ~EGUISection();

   virtual void paint( juce::Graphics &g );
   virtual void resized();

   virtual void samplesUpdated();

   virtual void sliderValueChanged( Slider *pSlider );

   void egUpdated( ENV *pENV );

protected:
   virtual ENV *getENV( const Sample *pSample ) const = 0;

private:
   juce::Slider *m_psAttack;
   juce::Label *m_plAttack;

   juce::Slider *m_psDecay;
   juce::Label *m_plDecay;

   juce::Slider *m_psSustain;
   juce::Label *m_plSustain;

   juce::Slider *m_psRelease;
   juce::Label *m_plRelease;
};
