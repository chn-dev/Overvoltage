#include <string>

#include "Part.h"
#include "util.h"

using namespace SamplerEngine;

Part::Part( size_t partNum ) :
   m_PartNum( partNum )
{
}


Part::~Part()
{
}


juce::XmlElement *Part::getStateInformation() const
{
   juce::XmlElement *pePart = new juce::XmlElement( "part" );
   pePart->setAttribute( "num", stdformat( "{}", m_PartNum ) );

   juce::XmlElement *peSamples = new juce::XmlElement( "samples" );
   for( Sample *pSample : m_Samples )
   {
      juce::XmlElement *peSample = pSample->getStateInformation();
      peSamples->addChildElement( peSample );

   }
   pePart->addChildElement( peSamples );

   return( pePart );
}


Part *Part::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "part" )
      return( nullptr );

   size_t partNum = std::stoul( pe->getStringAttribute( "num" ).toStdString() );

   Part *pPart = new Part( partNum );

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *peChild = pe->getChildElement( i );
      if( peChild->getTagName() == "samples" )
      {
         for( int nSample = 0; peChild->getChildElement( nSample ); nSample++ )
         {
            if( peChild->getChildElement( nSample )->getTagName() == "sample" )
            {
               juce::XmlElement *peSample = peChild->getChildElement( nSample );
               Sample *pSample = Sample::fromXml( peSample );
               if( pSample )
               {
                  pPart->m_Samples.push_back( pSample );
               }
            }
         }
      }
   }

   return( pPart );
}


size_t Part::getPartNum() const
{
   return( m_PartNum );
}


std::list<Sample *> &Part::samples()
{
   return( m_Samples );
}


const std::list<Sample *> &Part::constSamples() const
{
   return( m_Samples );
}


void Part::deleteSample( Sample *pSample )
{
   if( std::find( m_Samples.begin(), m_Samples.end(), pSample ) != m_Samples.end() )
   {
      m_Samples.remove( pSample );
      delete pSample;
   }
}
