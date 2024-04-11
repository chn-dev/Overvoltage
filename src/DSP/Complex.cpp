#include "Complex.h"

using namespace DSP;

Complex::Complex() :
   m_Valid( false ),
   m_A( 0.0 ),
   m_B( 0.0 )
{
}


Complex::Complex( double a, double b ) :
   m_Valid( true ),
   m_A( a ),
   m_B( b )
{
}


Complex::~Complex()
{
}


Complex::Complex( const Complex &d ) :
   m_Valid( d.m_Valid ),
   m_A( d.m_A ),
   m_B( d.m_B )
{
}


Complex Complex::operator=( const Complex &d )
{
   m_Valid = d.m_Valid;
   m_A = d.m_A;
   m_B = d.m_B;
   
   return( *this );
}


bool Complex::isValid() const
{
   return( m_Valid );
}


Complex Complex::fromCartesian( double a, double b )
{
   return( Complex( a, b ) );
}


Complex Complex::fromPolar( double r, double phi )
{
   return( Complex( r * cos( phi ), r * sin( phi ) ) );
}


double Complex::getA() const
{
   return( m_A );
}


double Complex::getB() const
{
   return( m_B );
}


double Complex::getR() const
{
   return( sqrt( ( m_A * m_A ) + ( m_B * m_B ) ) );
}


double Complex::getPhi() const
{
   return( atan2( m_B, m_A ) );
}


Complex Complex::operator+( const Complex &d ) const
{
   if( m_Valid && d.m_Valid )
   {
      return( Complex::fromCartesian( m_A + d.m_A, m_B + d.m_B ) );
   } else
   {
      return( Complex() );
   }
}


Complex Complex::operator+=( const Complex &d )
{
   if( m_Valid && d.m_Valid )
   {
      m_A += d.m_A;
      m_B += d.m_B;
   }

   return( *this );
}


Complex Complex::operator-( const Complex &d ) const
{
   if( m_Valid && d.m_Valid )
   {
      return( Complex::fromCartesian( m_A - d.m_A, m_B - d.m_B ) );
   } else
   {
      return( Complex() );
   }
}


Complex Complex::operator-=( const Complex &d )
{
   if( m_Valid && d.m_Valid )
   {
      m_A -= d.m_A;
      m_B -= d.m_B;
   }

   return( *this );
}


Complex Complex::operator*( const Complex &d ) const
{
   if( m_Valid && d.m_Valid )
   {
      double r = getR() * d.getR();
      double phi = getPhi() + d.getPhi();
      return( Complex::fromPolar( r, phi ) );
   } else
   {
      return( Complex() );
   }
}


Complex Complex::operator*=( const Complex &d )
{
   if( m_Valid && d.m_Valid )
   {
      double r = getR() * d.getR();
      double phi = getPhi() + d.getPhi();
      m_A = r * cos( phi );
      m_B = r * sin( phi );
   }

   return( *this );
}


Complex Complex::operator/( const Complex &d ) const
{
   if( m_Valid && d.m_Valid )
   {
      double r = getR() / d.getR();
      double phi = getPhi() - d.getPhi();
      return( Complex::fromPolar( r, phi ) );
   } else
   {
      return( Complex() );
   }
}


Complex Complex::operator/=( const Complex &d )
{
   if( m_Valid && d.m_Valid )
   {
      double r = getR() / d.getR();
      double phi = getPhi() - d.getPhi();
      m_A = r * cos( phi );
      m_B = r * sin( phi );
   }

   return( *this );
}
