/*----------------------------------------------------------------------------*/
/*!
\file UISectionEG.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionEG.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONEG_H__
#define __UISECTIONEG_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionEG
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionEG : public UISection,
                       public juce::Slider::Listener
   {
   public:
      UISectionEG( UIPage *pUIPage, std::string label );
      ~UISectionEG();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

      virtual void sliderValueChanged( Slider *pSlider );

      void egUpdated( SamplerEngine::ENV *pENV );

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const = 0;

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
}
#endif
