#define _USE_MATH_DEFINES
#include <math.h>
#include <util.h>

#include "Filter.h"

using namespace SamplerEngine;

Filter::Filter( const Filter &d )
{
   m_Cutoff = d.m_Cutoff;
   m_Resonance = d.m_Resonance;
   m_Type = d.m_Type;

   m_X[0] = 0.0;
   m_X[1] = 0.0;
   m_Y[0] = 0.0;
   m_Y[1] = 0.0;
}


Filter::Filter() :
   m_Cutoff( 1000.0 ),
   m_Resonance( 0.0 ),
   m_Type( TYPE_NONE )
{
   setCutoff( 1000.0f );
   setResonance( 0.1f );
   setType( TYPE_LOWPASS );

   m_X[0] = 0.0;
   m_X[1] = 0.0;
   m_Y[0] = 0.0;
   m_Y[1] = 0.0;
}


Filter::~Filter()
{
}


void Filter::setType( Filter::Type type )
{
   m_Type = type;
}


Filter::Type Filter::getType() const
{
   return( m_Type );
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
      } else
      if( tagName == "type" )
      {
         pFilter->m_Type = fromString( pChild->getChildElement( 0 )->getText().toStdString() );
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

   juce::XmlElement *peType = new juce::XmlElement( "type" );
   peType->addTextElement( toString( m_Type ) );
   pe->addChildElement( peType );

   return( pe );
}


void Filter::process( float *pSamples, const uint32_t n, double sampleRate, Type type, double cutoff, double resonance, double *pX, double *pY )
{
   double a1 = 1.0;
   double a2 = 0.0;
   double a3 = 0.0;
   double b1 = 0.0;
   double b2 = 0.0;

   double f = cutoff * 22050.0;
   double r = ( ( sqrt( 2.0 ) - 0.1 ) * ( 1.0 - resonance ) ) + 0.1;

   if( type == TYPE_LOWPASS )
   {
      // c = 1.0 / tan( pi * f / sample_rate );

      // a1 = 1.0 / ( 1.0 + r * c + c * c );
      // a2 = 2 * a1;
      // a3 = a1;
      // b1 = 2.0 * ( 1.0 - c * c ) * a1;
      // b2 = ( 1.0 - r * c + c * c ) * a1;

      double c = 1.0 / tan( M_PI * f / sampleRate );

      a1 = 1.0 / ( 1.0 + ( r * c ) + ( c * c ) );
      a2 = 2.0 * a1;
      a3 = a1;
      b1 = 2.0 * ( 1.0 - ( c * c ) ) * a1;
      b2 = ( 1.0 - ( r * c ) + ( c * c ) ) * a1;
   } else
   if( type == TYPE_HIGHPASS )
   {
      // c = tan( pi * f / sample_rate );

      // a1 = 1.0 / ( 1.0 + r * c + c * c );
      // a2 = -2 * a1;
      // a3 = a1;
      // b1 = 2.0 * ( c * c - 1.0 ) * a1;
      // b2 = ( 1.0 - r * c + c * c ) * a1;

      double c = tan( M_PI * f / sampleRate );

      a1 = 1.0 / ( 1.0 + ( r * c ) + ( c * c ) );
      a2 = -2.0 * a1;
      a3 = a1;
      b1 = 2.0 * ( ( c * c ) - 1.0 ) * a1;
      b2 = ( 1.0 - ( r * c ) + ( c * c ) ) * a1;
   }

   for( uint32_t i = 0; i < n; i++ )
   {
      float result = ( pSamples[i] * a1 ) + ( pX[0] * a2 ) + ( pX[1] * a3 ) -
         ( pY[0] * b1 ) - ( pY[1] * b2 );
      pY[1] = pY[0];
      pY[0] = result;
      pX[1] = pX[0];
      pX[0] = pSamples[i];
      pSamples[i] = result;
   }
}


void Filter::process( float *samples, const uint32_t n, double sampleRate )
{
   process( samples, n, sampleRate, m_Type, m_Cutoff, m_Resonance, m_X, m_Y );
}


std::string Filter::toString( Filter::Type type )
{
   if( type == TYPE_HIGHPASS )
   {
      return( "HighPass" );
   } else
   if( type == TYPE_LOWPASS )
   {
      return( "LowPass" );
   } else
   {
      return( "None" );
   }
}


Filter::Type Filter::fromString( const std::string &str )
{
   if( util::trim( util::toLower( str ) ) == "highpass" )
   {
      return( TYPE_HIGHPASS );
   } else
   if( util::trim( util::toLower( str ) ) == "lowpass" )
   {
      return( TYPE_LOWPASS );
   } else
   {
      return( TYPE_NONE );
   }
}


std::vector<Filter::Type> Filter::allTypes()
{
   return( std::vector<Filter::Type>( { TYPE_HIGHPASS, TYPE_LOWPASS, TYPE_NONE } ) );
}
