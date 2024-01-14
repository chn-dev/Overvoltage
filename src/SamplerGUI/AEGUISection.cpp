#include "AEGUISection.h"

using namespace SamplerGUI;

AEGUISection::AEGUISection( UIPage *pUIPage ) :
   EGUISection( pUIPage, "AEG" )
{
}


AEGUISection::~AEGUISection()
{
}

SamplerEngine::ENV *AEGUISection::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getAEG() );
}
