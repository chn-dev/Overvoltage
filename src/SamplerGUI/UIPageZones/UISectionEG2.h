/*----------------------------------------------------------------------------*/
/*!
\file UISectionEG2.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionEG2.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONEG2_H__
#define __UISECTIONEG2_H__

#include "JuceHeader.h"

#include "UISectionEG.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionEG2
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
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
