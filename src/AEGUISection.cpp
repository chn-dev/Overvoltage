#include "PluginEditor.h"
#include "AEGUISection.h"


AEGUISection::AEGUISection( PluginEditor *pEditor ) :
   EGUISection( pEditor, "AEG" )
{
}


AEGUISection::~AEGUISection()
{
}

Overvoltage::ENV *AEGUISection::getENV( const Overvoltage::Sample *pSample ) const
{
   return( pSample->getAEG() );
}
