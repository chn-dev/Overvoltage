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


/*----------------------------------------------------------------------------*/
/*!
\file ENV.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class ENV.
*/
/*----------------------------------------------------------------------------*/
#ifndef __ENV_H__
#define __ENV_H__

#include <set>
#include <libxml/tree.h>

//==============================================================================
namespace SamplerEngine
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class ENV
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class ENV
   {
   public:
      ENV();
      ~ENV();

      void getSettings( const ENV &d );

      enum State
      {
         StateStart,
         StateAttack,
         StateDecay,
         StateSustain,
         StateRelease,
         StateEnd,
         StateNone
      };

      void setAttack( double a );
      double getAttack() const;
      void setDecay( double d );
      double getDecay() const;
      void setSustain( double s );
      double getSustain() const;
      void setRelease( double r );
      double getRelease() const;

      double getValue() const;

      bool hasEnded() const;

      static double paramToDuration( double p );


      void noteOn();
      void noteOff();

      void step( double s, double bpm );

      static ENV *fromXml( xmlNode *pe );
      xmlNode *toXml() const;

   protected:

   private:
      double m_Value;
      State m_State;
      double m_Attack;
      double m_Decay;
      double m_Sustain;
      double m_Release;
   };
}
#endif

