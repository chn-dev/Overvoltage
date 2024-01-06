#include "PluginEditor.h"
#include "AEGUISection.h"


AEGUISection::AEGUISection( AudioPluginAudioProcessorEditor *pEditor ) :
   EGUISection( pEditor, "AEG" )
{
}


AEGUISection::~AEGUISection()
{
}

ENV *AEGUISection::getENV( const Sample *pSample ) const
{
   return( pSample->getAEG() );
}
