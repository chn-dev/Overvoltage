/*----------------------------------------------------------------------------*/
/*!
\file UISectionAEG.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionAEG.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONAEG_H__
#define __UISECTIONAEG_H__

#include "JuceHeader.h"

#include "UISectionEG.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionAEG
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionAEG : public UISectionEG
   {
   public:
      UISectionAEG( UIPage *pUIPage );
      ~UISectionAEG();

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const;
   };
}
#endif
