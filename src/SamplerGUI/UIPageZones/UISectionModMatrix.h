#ifndef __UISECTIONMODMATRIX_H__
#define __UISECTIONMODMATRIX_H__

#include "JuceHeader.h"

#include "UISectionEG.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class UISectionModMatrix : public UISection
   {
   public:
      UISectionModMatrix( UIPage *pUIPage );
      ~UISectionModMatrix();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

   protected:
   };
}
#endif
