#ifndef __UIPAGE_H__
#define __UIPAGE_H__

#include "JuceHeader.h"
#include "SamplerGUI.h"
#include "UISection.h"

class PluginEditor;

namespace SamplerGUI
{
   class UISection;

   class UIPage : public juce::Component
   {
   public:
      UIPage( PluginEditor *pEditor );
      ~UIPage();

      virtual void currentPartChanged( size_t nPart ) = 0;
      void onSampleSelectionUpdated( UISectionSamplerKeyboard *pKeyboard );

      virtual void paint( juce::Graphics &g ) override;
      virtual void resized() override;

      PluginEditor *editor() const;

      void addUISection( SamplerGUI::UISection *pSection );
      void removeUISection( SamplerGUI::UISection *pSection );

   protected:
      void updateUISections();

   protected:
      PluginEditor *m_pEditor;

   private:
      std::set<SamplerGUI::UISection *> m_UISections;
   };
}

#endif
