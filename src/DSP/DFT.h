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


#ifndef __DFT_H__
#define __DFT_H__

#include <vector>

#include "Wave.h"
#include "Complex.h"

namespace DSP
{
   class DFT
   {
      public:
         enum WindowType
         {
            WindowNone,
            WindowHamming
         };

         static bool dft( const Wave &wave, int nChannel, uint32_t nStart, uint32_t nLength, std::vector<Complex> &d, WindowType windowType = WindowNone  );
   };
}

#endif

