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
\file util.cpp
\author Christian Nowak <chnowak@web.de>
\brief Some utility functions
*/
/*----------------------------------------------------------------------------*/

#include "util.h"

static const char kEncodeLookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char kPadCharacter = '=';

namespace util
{
   /*----------------------------------------------------------------------------*/
   /*! 2023-12-15
   \param a A vector of doubles
   \return The index of the double with the highest value within the vector
   */
   /*----------------------------------------------------------------------------*/
   int indexOfMaxValue( const std::vector<double> &a )
   {
      if( a.size() < 1 )
      {
         return( -1 );
      }

      int r = 0;
      double v = a[0];

      for( size_t i = 0; i < a.size(); i++ )
      {
         if( a[i] > v )
         {
            v = a[i];
            r = (int)i;
         }
      }

      return( r );
   }


   /*----------------------------------------------------------------------------*/
   /*! 2023-12-15
   \param s The string to be trimmed
   \return The string with whitespaces trimmed from both sides
   */
   /*----------------------------------------------------------------------------*/
   std::string trim( std::string s )
   {
      std::string ws = "\r\n\t ";
      size_t pos = s.find_first_not_of( ws );
      if( pos != std::string::npos )
      {
         s = s.substr( pos );
      }

      pos = s.find_first_of( ws );
      if( pos != std::string::npos )
      {
         s = s.substr( 0, pos );
      }

      return( s );
   }


   std::string strjoin( std::vector<std::string> s, std::string sep )
   {
      std::string r = "";
      for( size_t i = 0; i < s.size(); i++ )
      {
         if( i > 0 )
            r += sep;
         r += s[i];
      }

      return( r );
   }


   /*----------------------------------------------------------------------------*/
   /*! 2023-12-15
   Split a string into substrings where a given separator occurs.
   \param str The string to split
   \param sep The separator
   \param keepEmpty If false, empty substrings will be omitted
   \return The vector of substrings
   */
   /*----------------------------------------------------------------------------*/
   std::vector<std::string> strsplit( std::string str, std::string sep, bool keepEmpty )
   {
      std::vector<std::string> r;

      size_t pos;
      size_t curPos = 0;
      while( ( pos = str.find( sep, curPos ) ) != std::string::npos )
      {
         std::string s = str.substr( curPos, pos - curPos );
         if( keepEmpty || s.size() > 0 )
         {
            r.push_back( s );
         }
         curPos = pos + sep.size();
      }

      if( curPos <= str.size() )
      {
         std::string s = str.substr( curPos );
         if( keepEmpty || s.size() > 0 )
         {
            r.push_back( s );
         }
      }
      return( r );
   }


   /*----------------------------------------------------------------------------*/
   /*! 2023-12-15
   Generate a random floating point value within given limits.
   \param min The lower limit
   \param max The upper limit
   \return The generated random value
   */
   /*----------------------------------------------------------------------------*/
   double randomValue( double min, double max )
   {
      if( max < min )
      {
         double tmp = min;
         min = max;
         max = tmp;
      }

      double v = (double)std::rand() / (double)RAND_MAX; // v = [0.0 .. 1.0]

      return( ( v * ( max - min ) ) + min );
   }

   std::string toLower( const std::string &str )
   {
      std::string r = str;
      for( size_t i = 0; i < r.length(); i++ )
      {
         r[i] = (char)std::tolower( str[i] );
      }

      return( r );
   }

   double clamp( double min, double max, double v )
   {
      if( v < min )
         return( min );
      else
      if( v > max )
         return( max );
      else
         return( v );
   }


   std::string base64encode( const std::vector<uint8_t> &input )
   {
      std::string encoded;
      encoded.reserve( ( ( input.size() / 3 ) + ( input.size() % 3 > 0 ) ) * 4 );

      std::uint32_t temp{};
      auto it = input.begin();

      for( std::size_t i = 0; i < input.size() / 3; ++i )
      {
         temp  = (uint32_t)(*it++) << 16;
         temp += (uint32_t)(*it++) << 8;
         temp += (uint32_t)(*it++);
         encoded.append( 1, kEncodeLookup[( temp & 0x00FC0000 ) >> 18] );
         encoded.append( 1, kEncodeLookup[( temp & 0x0003F000 ) >> 12] );
         encoded.append( 1, kEncodeLookup[( temp & 0x00000FC0 ) >> 6 ] );
         encoded.append( 1, kEncodeLookup[( temp & 0x0000003F )      ] );
      }

      switch( input.size() % 3 )
      {
         case 1:
            temp = (uint32_t)(*it++) << 16;
            encoded.append( 1, kEncodeLookup[( temp & 0x00FC0000 ) >> 18] );
            encoded.append( 1, kEncodeLookup[( temp & 0x0003F000 ) >> 12] );
            encoded.append( 2, kPadCharacter );
            break;
         case 2:
            temp  = (uint32_t)(*it++) << 16;
            temp += (uint32_t)(*it++) << 8;
            encoded.append( 1, kEncodeLookup[( temp & 0x00FC0000 ) >> 18] );
            encoded.append( 1, kEncodeLookup[( temp & 0x0003F000 ) >> 12] );
            encoded.append( 1, kEncodeLookup[( temp & 0x00000FC0 ) >> 6 ] );
            encoded.append( 1, kPadCharacter );
            break;
      }

      return( encoded );
   }


   std::vector<uint8_t> base64decode( const std::string &input )
   {
      if( input.length() % 4 )
         return( std::vector<uint8_t>() );

      std::size_t padding{};

      if( input.length() )
      {
         if( input[input.length() - 1] == kPadCharacter )
            padding++;
         if( input[input.length() - 2] == kPadCharacter )
            padding++;
      }

      std::vector<uint8_t> decoded;
      decoded.reserve( ( ( input.length() / 4 ) * 3 ) - padding );

      std::uint32_t temp{};
      auto it = input.begin();

      while( it < input.end() )
      {
         for( std::size_t i = 0; i < 4; ++i )
         {
            temp <<= 6;
            if     ( *it >= 0x41 && *it <= 0x5A ) temp |= (uint32_t)(*it) - 0x41;
            else if( *it >= 0x61 && *it <= 0x7A ) temp |= (uint32_t)(*it) - 0x47;
            else if( *it >= 0x30 && *it <= 0x39 ) temp |= (uint32_t)(*it) + 0x04;
            else if( *it == 0x2B )                temp |= 0x3E;
            else if( *it == 0x2F )                temp |= 0x3F;
            else if( *it == kPadCharacter )
            {
               switch( input.end() - it )
               {
                  case 1:
                     decoded.push_back( ( temp >> 16 ) & 0x000000FF );
                     decoded.push_back( ( temp >> 8  ) & 0x000000FF );
                     return( decoded );
                  case 2:
                     decoded.push_back( ( temp >> 10 ) & 0x000000FF );
                     return( decoded );
                  default:
                     return( std::vector<uint8_t>() );
               }
            } else
            {
               return( std::vector<uint8_t>() );
            }

            ++it;
         }

         decoded.push_back( ( temp >> 16 ) & 0x000000FF );
         decoded.push_back( ( temp >> 8  ) & 0x000000FF );
         decoded.push_back( ( temp       ) & 0x000000FF );
      }

      return( decoded );
   }

   std::string toString( xmlNode *pXml )
   {
      xmlDoc *pDoc = xmlNewDoc( (xmlChar *)"1.0" );
      xmlDocSetRootElement( pDoc, pXml );
      xmlChar *pBuf;
      int bufSize;
      xmlDocDumpFormatMemory( pDoc, &pBuf, &bufSize, 1 );
      std::string str = std::string( (char *)pBuf );
      xmlFree( pBuf );
      xmlFreeDoc( pDoc );

      return( str );
   }
}
