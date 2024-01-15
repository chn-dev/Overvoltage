#include "UISectionAEG.h"

using namespace SamplerGUI;

UISectionAEG::UISectionAEG( UIPage *pUIPage ) :
   UISectionEG( pUIPage, "AEG" )
{
}


UISectionAEG::~UISectionAEG()
{
}

SamplerEngine::ENV *UISectionAEG::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getAEG() );
}
