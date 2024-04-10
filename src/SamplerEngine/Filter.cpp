#include <math.h>
#include <util.h>

#include "Filter.h"

using namespace SamplerEngine;

Filter::Filter() :
   m_Cutoff( 1.0 ),
   m_Resonance( 0.0 )
{
}


Filter::~Filter()
{
}


double Filter::getCutoff() const
{
   return( m_Cutoff );
}


double Filter::getResonance() const
{
   return( m_Resonance );
}


void Filter::setCutoff( double v )
{
   m_Cutoff = v;
}


void Filter::setResonance( double v )
{
   m_Resonance = v;
}


Filter *Filter::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "filter" )
      return( nullptr );

   Filter *pFilter = new Filter();

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "cutoff" )
      {
         pFilter->m_Cutoff = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "resonance" )
      {
         pFilter->m_Resonance = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      }
   }

   return( pFilter );
}


juce::XmlElement *Filter::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "filter" );

   juce::XmlElement *peCutoff = new juce::XmlElement( "cutoff" );
   peCutoff->addTextElement( stdformat( "{}", m_Cutoff ) );
   pe->addChildElement( peCutoff );

   juce::XmlElement *peResonance = new juce::XmlElement( "resonance" );
   peResonance->addTextElement( stdformat( "{}", m_Resonance ) );
   pe->addChildElement( peResonance );

   return( pe );
}
