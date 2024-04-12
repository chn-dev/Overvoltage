#include <string>

#include "ModMatrix.h"

#include <util.h>

using namespace SamplerEngine;

ModMatrix::ModMatrix()
{
}


ModMatrix::~ModMatrix()
{
}


ModMatrix *ModMatrix::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "modmatrix" )
      return( nullptr );

   ModMatrix *pModMatrix = new ModMatrix();

   return( pModMatrix );
}


juce::XmlElement *ModMatrix::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "modmatrix" );

   return( pe );
}
