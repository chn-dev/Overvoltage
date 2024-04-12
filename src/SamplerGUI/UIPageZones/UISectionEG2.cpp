#include "UISectionEG2.h"

using namespace SamplerGUI;

UISectionEG2::UISectionEG2( UIPage *pUIPage ) :
   UISectionEG( pUIPage, "EG2" )
{
}


UISectionEG2::~UISectionEG2()
{
}

SamplerEngine::ENV *UISectionEG2::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getEG2() );
}
