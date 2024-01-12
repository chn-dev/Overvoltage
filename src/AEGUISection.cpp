#include "PluginEditor.h"
#include "AEGUISection.h"


AEGUISection::AEGUISection( PluginEditor *pEditor ) :
   EGUISection( pEditor, "AEG" )
{
}


AEGUISection::~AEGUISection()
{
}

ENV *AEGUISection::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getAEG() );
}
