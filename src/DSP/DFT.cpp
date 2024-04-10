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
