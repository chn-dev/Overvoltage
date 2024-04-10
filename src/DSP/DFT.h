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
