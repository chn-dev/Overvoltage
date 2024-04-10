#ifndef __UIPAGEZONES_H__
#define __UIPAGEZONES_H__

#include <SamplerGUI/UIPage.h>

#include "UISectionWaveView.h"
#include "UISectionSample.h"
#include "UISectionNameRanges.h"
#include "UISectionAEG.h"
#include "UISectionFilter.h"
#include "UISectionOutput.h"
#include "UISectionSamplerKeyboard.h"

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
      SamplerGUI::UISectionSamplerKeyboard *m_pUISectionKeyboard;
      SamplerGUI::UISectionWaveView *m_pUISectionWaveView;
      SamplerGUI::UISectionSample *m_pUISectionSample;
      SamplerGUI::UISectionNameRanges *m_pUISectionNameRanges;
      SamplerGUI::UISectionAEG *m_pUISectionAEG;
      SamplerGUI::UISectionFilter *m_pUISectionFilter;
      SamplerGUI::UISectionOutput *m_pUISectionOutput;
   };
}

#endif
