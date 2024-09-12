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


#define _USE_MATH_DEFINES
#include "DFT.h"

using namespace DSP;

bool DFT::dft( const Wave &wave, int nChannel, uint32_t nStart, uint32_t nLength, std::vector<Complex> &d, WindowType windowType )
{
   if( nStart >= wave.numSamples() || nStart + nLength >= wave.numSamples() ||
       nChannel < 0 || nChannel >= wave.numChannels() )
      return( false );

   d.clear();
   uint32_t N = nLength;
   uint32_t end = N - 1;
   std::vector<double> db;
   for( uint32_t nSample = nStart; nSample <= end; nSample++ )
   {
      uint32_t k = nSample - nStart;
      DSP::Complex ak = DSP::Complex();
      double wArea = 0.0;
      for( uint32_t j = 0; j < N; j++ )
      {
         double w = 1.0;
         if( windowType == WindowHamming )
            //w = ( ( 25.0 / 46.0 ) - 0.46164 * cos(2.0 * M_PI * j / ( ( N ) )) );
            w = ( ( 0.54 ) - 0.46 * cos(2.0 * M_PI * j / ( ( N ) )) );
         wArea += w;
         double v = wave.floatValue( nChannel, j ) * w;
         DSP::Complex cpnt = DSP::Complex::fromPolar( (double)v, -2.0 * M_PI * (double)j * (double)k / (double)(N) );
         if( j == 0 )
         {
            ak = cpnt;
         } else
         {
            ak += cpnt;
         }
      }
      d.push_back( ak );
      db.push_back( 20.0 * log10( ak.getR() / wArea ) );
   }

   return( true );
}

