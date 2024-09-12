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
