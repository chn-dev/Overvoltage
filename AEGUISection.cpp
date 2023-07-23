#include "PluginEditor.h"
#include "AEGUISection.h"


AEGUISection::AEGUISection( AudioPluginAudioProcessorEditor *pEditor ) :
   EGUISection( pEditor, "AEG" )
{
}


AEGUISection::~AEGUISection()
{
}


void AEGUISection::paint( juce::Graphics &g )
{
   EGUISection::paint( g );
}


void AEGUISection::resized()
{
   EGUISection::resized();
}

   
void AEGUISection::sampleUpdated()
{
   EGUISection::sampleUpdated();

   if( sample() )
   {
      egUpdated( sample()->getAEG() );
   }
}
