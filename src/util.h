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
\file util.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for some utility functions
*/
/*----------------------------------------------------------------------------*/
#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>

#include <libxml/tree.h>

#ifdef __GNUC__
   #if __cplusplus >= 202002L
      #include <format>
      #define stdformat std::format
      #define stdvformat(f, ...) std::vformat( std::string_view( f ), std::make_format_args( __VA_ARGS__ ) )
   #else
      #include <fmt/core.h>
      #define stdformat fmt::format
      #define stdvformat(f, ...) fmt::format( f, __VA_ARGS__ )
   #endif
#else
   #include <format>
   #define stdformat std::format
   #define stdvformat(fmt, ...) std::vformat( std::string_view( fmt ), std::make_format_args( ##__VA_ARGS__ ) )
#endif

namespace util
{
   int indexOfMaxValue( const std::vector<double> &a );
   std::string trim( std::string s );
   std::vector<std::string> strsplit( std::string str, std::string sep, bool keepEmpty );
   std::string strjoin( std::vector<std::string> s, std::string sep );
   double randomValue( double min, double max );
   std::string toLower( const std::string &str );
   double clamp( double min, double max, double v );
   std::vector<uint8_t> base64decode( const std::string &input );
   std::string base64encode( const std::vector<uint8_t> &input );
   std::string toString( xmlNode *pXml );
}

#endif
