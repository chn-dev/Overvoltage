/*----------------------------------------------------------------------------*/
/*!
\file UIPageZones.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UIPageZones.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UIPAGEZONES_H__
#define __UIPAGEZONES_H__

#include <SamplerGUI/UIPage.h>

#include "UISectionWaveView.h"
#include "UISectionSample.h"
#include "UISectionNameRanges.h"
#include "UISectionAEG.h"
#include "UISectionEG2.h"
#include "UISectionLFO.h"
#include "UISectionFilter.h"
#include "UISectionOutput.h"
#include "UISectionModMatrix.h"
#include "UISectionSamplerKeyboard.h"

class PluginEditor;

namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UIPageZones
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UIPageZones : public UIPage,
                       public juce::Button::Listener
   {
   public:
      UIPageZones( PluginEditor *pEditor );
      ~UIPageZones();

      virtual void currentPartChanged( size_t nPart );
      virtual void buttonClicked( Button *pButton );
      virtual void buttonStateChanged( Button *pButton );

      int getCurrentLayer() const;
      void setCurrentLayer( int nLayer );

      SamplerGUI::UISectionSamplerKeyboard *getSamplerKeyboard() const;

      bool isSoloEnabled() const;

   protected:
      SamplerGUI::UISectionSamplerKeyboard *m_pUISectionKeyboard;
      SamplerGUI::UISectionWaveView *m_pUISectionWaveView;
      SamplerGUI::UISectionSample *m_pUISectionSample;
      SamplerGUI::UISectionNameRanges *m_pUISectionNameRanges;
      SamplerGUI::UISectionAEG *m_pUISectionAEG;
      SamplerGUI::UISectionEG2 *m_pUISectionEG2;
      SamplerGUI::UISectionLFO *m_pUISectionLFO;
      SamplerGUI::UISectionFilter *m_pUISectionFilter;
      SamplerGUI::UISectionOutput *m_pUISectionOutput;
      SamplerGUI::UISectionModMatrix *m_pUISectionModMatrix;

      std::vector<juce::TextButton *> m_LayerButtons;
      juce::TextButton *m_pbSolo;
      juce::TextButton *m_pbSelLayer;
      juce::TextButton *m_pbSelAll;
   };
}

#endif
