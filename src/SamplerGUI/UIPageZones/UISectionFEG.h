#ifndef __UISECTIONFEG_H__
#define __UISECTIONFEG_H__

#include "JuceHeader.h"

#include "UISectionEG.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class UISectionFEG : public UISectionEG
   {
   public:
      UISectionFEG( UIPage *pUIPage );
      ~UISectionFEG();

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const;
   };
}
#endif
