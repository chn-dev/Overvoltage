#include "UISectionModMatrix.h"

using namespace SamplerGUI;

UISectionModMatrix::UISectionModMatrix( UIPage *pUIPage ) :
   UISection( pUIPage, "Modulation Matrix" )
{
}


UISectionModMatrix::~UISectionModMatrix()
{
}


void UISectionModMatrix::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void UISectionModMatrix::resized()
{
   UISection::resized();
}


void UISectionModMatrix::samplesUpdated()
{
}
