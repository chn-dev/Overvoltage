#include "UISectionFEG.h"

using namespace SamplerGUI;

UISectionFEG::UISectionFEG( UIPage *pUIPage ) :
   UISectionEG( pUIPage, "FEG" )
{
}


UISectionFEG::~UISectionFEG()
{
}

SamplerEngine::ENV *UISectionFEG::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getFEG() );
}
