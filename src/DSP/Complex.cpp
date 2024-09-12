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

