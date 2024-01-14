#ifndef __UIPAGEZONES_H__
#define __UIPAGEZONES_H__

#include "SamplerGUI.h"
#include "UIPage.h"

#include "WaveView.h"
#include "SampleUISection.h"
#include "NameRangesUISection.h"
#include "AEGUISection.h"
#include "OutputUISection.h"

class PluginEditor;

namespace SamplerGUI
{
   class UIPageZones : public UIPage
   {
   public:
      UIPageZones( PluginEditor *pEditor );
      ~UIPageZones();

      virtual void currentPartChanged( size_t nPart );

   protected:
      SamplerGUI::SamplerKeyboard *m_pKeyboard;
      SamplerGUI::WaveView *m_pWaveView;
      SamplerGUI::SampleUISection *m_pSampleSection;
      SamplerGUI::NameRangesUISection *m_pNameRangesUISection;
      SamplerGUI::AEGUISection *m_pAEGUISection;
      SamplerGUI::OutputUISection *m_pOutputUISection;
   };
}

#endif
