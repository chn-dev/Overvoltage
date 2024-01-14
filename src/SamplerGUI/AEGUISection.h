#ifndef __AEGUISECTION_H__
#define __AEGUISECTION_H__

#include "JuceHeader.h"

#include "EGUISection.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class AEGUISection : public EGUISection
   {
   public:
      AEGUISection( UIPage *pUIPage );
      ~AEGUISection();

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const;
   };
}
#endif
