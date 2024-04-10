#ifndef __WAVE_H__
#define __WAVE_H__

#include <stdint.h>

namespace DSP
{
   class Wave
   {
      public:
         virtual float floatValue( int nChannel, uint32_t nSample ) const = 0;
         virtual int numChannels() const = 0;
         virtual uint32_t sampleRate() const = 0;
         virtual int numBits() const = 0;
         virtual uint32_t numSamples() const = 0;
   };
}

#endif
