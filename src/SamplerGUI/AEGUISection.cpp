#include "PluginEditor.h"

using namespace SamplerGUI;

AEGUISection::AEGUISection( PluginEditor *pEditor ) :
   EGUISection( pEditor, "AEG" )
{
}


AEGUISection::~AEGUISection()
{
}

SamplerEngine::ENV *AEGUISection::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getAEG() );
}
