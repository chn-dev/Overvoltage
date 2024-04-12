#ifndef __UISECTIONEG2_H__
#define __UISECTIONEG2_H__

#include "JuceHeader.h"

#include "UISectionEG.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class UISectionEG2 : public UISectionEG
   {
   public:
      UISectionEG2( UIPage *pUIPage );
      ~UISectionEG2();

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const;
   };
}
#endif
