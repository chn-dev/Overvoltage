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


#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <math.h>

namespace DSP
{
   class Complex
   {
      public:
         Complex();
         Complex( const Complex &d );
         ~Complex();
         
         bool isValid() const;
         
         double getA() const;
         double getB() const;
         double getR() const;
         double getPhi() const;

         Complex operator=( const Complex &d );
         
         Complex operator+( const Complex &d ) const;
         Complex operator+=( const Complex &d );
         Complex operator-( const Complex &d ) const;
         Complex operator-=( const Complex &d );

         Complex operator*( const Complex &d ) const;
         Complex operator*=( const Complex &d );
         Complex operator/( const Complex &d ) const;
         Complex operator/=( const Complex &d );


         static Complex fromCartesian( double a, double b );
         static Complex fromPolar( double r, double phi );
         
      private:
         Complex( double a, double b );
         
      private:
         bool m_Valid;
         double m_A;
         double m_B;
   };
}

#endif

