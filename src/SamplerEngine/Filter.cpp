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
   m_CutoffMod = d.m_CutoffMod;
   m_ResonanceMod = d.m_ResonanceMod;

   for( int c = 0; c < 2; c++ )
   {
      for( int n = 0; n < 2; n++ )
      {
         m_X[c][n] = 0.0;
         m_Y[c][n] = 0.0;
      }
   }
}


Filter::Filter() :
   m_Type( TYPE_NONE ),
   m_CutoffMod( 0.0 ),
   m_Cutoff( 1.0 ),
   m_Resonance( 0.0 ),
   m_ResonanceMod( 0.0 )
{
   for( int c = 0; c < 2; c++ )
   {
      for( int n = 0; n < 2; n++ )
      {
         m_X[c][n] = 0.0;
         m_Y[c][n] = 0.0;
      }
   }
}


Filter::~Filter()
{
}


void Filter::getSettings( const Filter &d )
{
   m_Type = d.m_Type;
   m_Cutoff = d.m_Cutoff;
   m_Resonance = d.m_Resonance;
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
   if( v > 1.0 )
      v = 1.0;
   else
   if( v < 0.0 )
      v = 0.0;

   m_Cutoff = v;
}


void Filter::setResonance( double v )
{
   if( v > 1.0 )
      v = 1.0;
   else
   if( v < 0.0 )
      v = 0.0;

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
         pFilter->setCutoff( std::stof( pChild->getChildElement( 0 )->getText().toStdString() ) );
      } else
      if( tagName == "resonance" )
      {
         pFilter->setResonance( std::stof( pChild->getChildElement( 0 )->getText().toStdString() ) );
      } else
      if( tagName == "type" )
      {
         pFilter->setType( fromString( pChild->getChildElement( 0 )->getText().toStdString() ) );
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


void Filter::process( float *pLSamples, float *pRSamples, const uint32_t n, double sampleRate, Type type, double cutoff, double resonance, double *pXL, double *pXR, double *pYL, double *pYR )
{
   std::vector<float *> samples( { pLSamples, pRSamples } );
   std::vector<double *> X( { pXL, pXR } );
   std::vector<double *> Y( { pYL, pYR } );

   double a1 = 1.0;
   double a2 = 0.0;
   double a3 = 0.0;
   double b1 = 0.0;
   double b2 = 0.0;

   double f = 1.0 + ( cutoff * ( 16000.0 - 1.0 ) );
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

   for( size_t nChan = 0; nChan < 2; nChan++ )
   {
      for( size_t i = 0; i < n; i++ )
      {
         float result =
            + ( samples[nChan][i] * a1 )
            +       ( X[nChan][0] * a2 )
            +       ( X[nChan][1] * a3 )
            -       ( Y[nChan][0] * b1 )
            -       ( Y[nChan][1] * b2 );
         Y[nChan][1] = Y[nChan][0];
         Y[nChan][0] = result;
         X[nChan][1] = X[nChan][0];
         X[nChan][0] = samples[nChan][i];
         samples[nChan][i] = result;
      }
   }
}


void Filter::process( float *pLSamples, float *pRSamples, const uint32_t n, double sampleRate )
{
   process(
      pLSamples, pRSamples, n, sampleRate,
      m_Type,
      util::clamp( 0.0, 1.0, m_Cutoff * pow( 2.0, m_CutoffMod ) ),
      util::clamp( 0.0, 1.0, m_Resonance + ( m_ResonanceMod / 100.0 ) ),
      m_X[0], m_X[1], m_Y[0], m_Y[1] );
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


std::set<Filter::Type> Filter::allTypes()
{
   return( std::set<Filter::Type>( { TYPE_HIGHPASS, TYPE_LOWPASS, TYPE_NONE } ) );
}


void Filter::setCutoffMod( double v )
{
   m_CutoffMod = v;
}


double Filter::getCutoffMod() const
{
   return( m_CutoffMod );
}


void Filter::setResonanceMod( double v )
{
   m_ResonanceMod = v;
}


double Filter::getResonanceMod() const
{
   return( m_ResonanceMod );
}
