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
}

#endif
