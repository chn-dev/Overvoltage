/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of chn's Overvoltage.                                   *
 *                                                                             *
 *  Overvoltage is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *          
 *  Overvoltage is distributed in the hope that it will be useful, but         * 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Overvoltage. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file Filter.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a Filter
*/
/*----------------------------------------------------------------------------*/
#define _USE_MATH_DEFINES
#include <math.h>
#include <util.h>

#include "Filter.h"

using namespace SamplerEngine;

/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Constructor
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Constructor
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Destructor
*/
/*----------------------------------------------------------------------------*/
Filter::~Filter()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Copy filter settings from another Filter object.
\param d
*/
/*----------------------------------------------------------------------------*/
void Filter::getSettings( const Filter &d )
{
   m_Type = d.m_Type;
   m_Cutoff = d.m_Cutoff;
   m_Resonance = d.m_Resonance;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\param type The new filter type
*/
/*----------------------------------------------------------------------------*/
void Filter::setType( Filter::Type type )
{
   m_Type = type;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\return The filter type
*/
/*----------------------------------------------------------------------------*/
Filter::Type Filter::getType() const
{
   return( m_Type );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\return The cutoff frequency (0..1)
*/
/*----------------------------------------------------------------------------*/
double Filter::getCutoff() const
{
   return( m_Cutoff );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\return The resonance (0..1)
*/
/*----------------------------------------------------------------------------*/
double Filter::getResonance() const
{
   return( m_Resonance );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\param v The new cutoff frequency (0..1)
*/
/*----------------------------------------------------------------------------*/
void Filter::setCutoff( double v )
{
   if( v > 1.0 )
      v = 1.0;
   else
   if( v < 0.0 )
      v = 0.0;

   m_Cutoff = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\param v The new resonance (0..1)
*/
/*----------------------------------------------------------------------------*/
void Filter::setResonance( double v )
{
   if( v > 1.0 )
      v = 1.0;
   else
   if( v < 0.0 )
      v = 0.0;

   m_Resonance = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Recreate the Filter from an XmlElement
\param pe Pointer to the XmlElement
\return The new Filter object
*/
/*----------------------------------------------------------------------------*/
Filter *Filter::fromXml( xmlNode *pe )
{
   if( std::string( (char*)pe->name ) != "filter" )
      return( nullptr );

   Filter *pFilter = new Filter();

   for( xmlNode *pChild = pe->children; pChild; pChild = pChild->next )
   {
      std::string tagName = std::string( (char*)pChild->name );

      if( tagName == "cutoff" )
      {
         pFilter->setCutoff( std::stof( std::string( (char*)pChild->children->content ) ) );
      } else
      if( tagName == "resonance" )
      {
         pFilter->setResonance( std::stof( std::string( (char*)pChild->children->content ) ) );
      } else
      if( tagName == "type" )
      {
         pFilter->setType( fromString( std::string( (char*)pChild->children->content ) ) );
      }
   }

   return( pFilter );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Create an XmlElement from the Filter settings
\return Pointer to the XmlElement
*/
/*----------------------------------------------------------------------------*/
xmlNode *Filter::toXml() const
{
   xmlNode *pe = xmlNewNode( nullptr, (xmlChar *)"filter" );

   xmlNode *peCutoff = xmlNewNode( nullptr, (xmlChar *)"cutoff" );
   xmlAddChild( peCutoff, xmlNewText( (xmlChar *)stdformat( "{}", m_Cutoff ).c_str() ) );
   xmlAddChild( pe, peCutoff );

   xmlNode *peResonance = xmlNewNode( nullptr, (xmlChar *)"resonance" );
   xmlAddChild( peResonance, xmlNewText( (xmlChar *)stdformat( "{}", m_Resonance ).c_str() ) );
   xmlAddChild( pe, peResonance );

   xmlNode *peType = xmlNewNode( nullptr, (xmlChar *)"type" );
   xmlAddChild( peType, xmlNewText( (xmlChar *)toString( m_Type ).c_str() ) );
   xmlAddChild( pe, peType );

   return( pe );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Apply the filter to audio data.
\param pLSamples Left audio channel
\param lRSamples Right audio channel
\param n Number of samples
\param sampleRate Sample rate in Hz
\param type Filter type
\param cutoff Cutoff frequency (0..1)
\param resonance Resonance (0..1)
*/
/*----------------------------------------------------------------------------*/
void Filter::process( float *pLSamples, float *pRSamples,
                      const uint32_t n, double sampleRate,
                      Type type, double cutoff, double resonance,
                      double *pXL, double *pXR, double *pYL, double *pYR )
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
         double result =
            + ( samples[nChan][i] * a1 )
            +       ( X[nChan][0] * a2 )
            +       ( X[nChan][1] * a3 )
            -       ( Y[nChan][0] * b1 )
            -       ( Y[nChan][1] * b2 );
         Y[nChan][1] = Y[nChan][0];
         Y[nChan][0] = result;
         X[nChan][1] = X[nChan][0];
         X[nChan][0] = samples[nChan][i];
         samples[nChan][i] = (float)result;
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Apply the filter to audio data.
\param pLSamples Left audio channel
\param pRSamples Right audio channel
\param n Number of samples
\param sampleRate Sample rate
*/
/*----------------------------------------------------------------------------*/
void Filter::process( float *pLSamples, float *pRSamples, const uint32_t n, double sampleRate )
{
   process(
      pLSamples, pRSamples, n, sampleRate,
      m_Type,
      util::clamp( 0.0, 1.0, m_Cutoff * pow( 2.0, m_CutoffMod ) ),
      util::clamp( 0.0, 1.0, m_Resonance + ( m_ResonanceMod / 100.0 ) ),
      m_X[0], m_X[1], m_Y[0], m_Y[1] );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Convert a filter type enum value to a string
\param type Filter type
\return A textual representation of the filter type
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
Convert the textual representation of a filter type to the corresponding enum value.
\param str The string
\return The enum value
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\return A list with all available filter types
*/
/*----------------------------------------------------------------------------*/
std::set<Filter::Type> Filter::allTypes()
{
   return( std::set<Filter::Type>( { TYPE_HIGHPASS, TYPE_LOWPASS, TYPE_NONE } ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\param v A mod value for the cutoff frequency
*/
/*----------------------------------------------------------------------------*/
void Filter::setCutoffMod( double v )
{
   m_CutoffMod = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\return The mod value for the cutoff frequency
*/
/*----------------------------------------------------------------------------*/
double Filter::getCutoffMod() const
{
   return( m_CutoffMod );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\param v A mod value for the resonance
*/
/*----------------------------------------------------------------------------*/
void Filter::setResonanceMod( double v )
{
   m_ResonanceMod = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-11
\return The mod value for the resonance
*/
/*----------------------------------------------------------------------------*/
double Filter::getResonanceMod() const
{
   return( m_ResonanceMod );
}

