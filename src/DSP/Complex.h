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
