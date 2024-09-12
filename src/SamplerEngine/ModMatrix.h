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
\file ModMatrix.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class ModMatrix.
*/
/*----------------------------------------------------------------------------*/
#ifndef __MODMATRIX_H__
#define __MODMATRIX_H__

#include <map>
#include <set>
#include <vector>

#include <libxml/tree.h>

#define SAMPLERENGINE_NUMMODSLOTS 5

//==============================================================================
namespace SamplerEngine
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class ModMatrix
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class ModMatrix
   {
   public:
      enum ModSrc
      {
         ModSrc_None = 1,
         ModSrc_AEG,
         ModSrc_EG2,
         ModSrc_Velocity,
         ModSrc_AbsNote,
         ModSrc_RelNote,
         ModSrc_LFO1,
         ModSrc_LFO2,
         ModSrc_LFO3,
         ModSrc_ModWheel,
         ModSrc_RandomUnipolar,
         ModSrc_RandomBipolar,
         ModSrc_Gate,
         ModSrc_IsWithinLoop
      };

      enum ModDest
      {
         ModDest_None = 1,
         ModDest_FilterCutoff,
         ModDest_FilterResonance,
         ModDest_Pitch,
         ModDest_Pan,
         ModDest_Amp
      };

      enum MathFunc
      {
         MathFunc_X = 1,
         MathFunc_OneMinusX,
         MathFunc_BiPolar2UniPolar,
         MathFunc_UniPolar2BiPolar,
         MathFunc_Squared,
         MathFunc_Cubed,
         MathFunc_Abs,
         MathFunc_Neg
      };

      /*----------------------------------------------------------------------------*/
      /*!
      \class ModDestInfo
      \date  2024-06-28
      */
      /*----------------------------------------------------------------------------*/
      class ModDestInfo
      {
      public:
         ModDestInfo( ModDest md );
         ModDestInfo(
            ModDest md,
            double minVal, double maxVal, double step, double defaultVal,
            std::string unit, std::string format );
         ModDestInfo();
         ~ModDestInfo();

         double getMin() const;
         double getMax() const;
         double getStep() const;
         ModDest getType() const;
         std::string getUnit() const;
         std::string getFormat() const;
         double getDefaultValue() const;

      protected:
         static std::map<ModDest, ModDestInfo> &infoMap();

      private:
         ModDest m_Type;
         double m_Min;
         double m_Max;
         double m_Step;
         double m_Default;
         std::string m_Unit;
         std::string m_Format;
      };


      /*----------------------------------------------------------------------------*/
      /*!
      \class ModSlot
      \date  2024-06-28
      */
      /*----------------------------------------------------------------------------*/
      class ModSlot
      {
      public:
         ModSlot( ModSrc src, ModSrc mod, ModDest dest, MathFunc func, double amt, bool enabled );
         ModSlot( const ModSlot &d );
         ModSlot();
         ~ModSlot();

         static ModSlot *fromXml( xmlNode *pe );
         xmlNode *toXml() const;

         ModSrc getSrc() const;
         void setSrc( ModSrc src );
         ModSrc getMod() const;
         void setMod( ModSrc mod );
         ModDest getDest() const;
         void setDest( ModDest dest );
         MathFunc getMathFunc() const;
         void setMathFunc( MathFunc f );
         double getAmount() const;
         void setAmount( double amount );
         bool isEnabled() const;
         void setEnabled( bool e );

      private:
         bool m_Enabled;
         ModSrc m_Src;
         ModSrc m_Mod;
         ModDest m_Dest;
         MathFunc m_Func;
         double m_Amt;
      };

      ModMatrix( size_t numSlots = 0 );
      ~ModMatrix();

      size_t numSlots() const;
      ModSlot *getSlot( size_t n ) const;

      static ModMatrix *fromXml( xmlNode *pe );
      xmlNode *toXml() const;

      static std::string toString( ModSrc v );
      static ModSrc modSrcFromString( const std::string &s );
      static std::string toString( ModDest v );
      static ModDest modDestFromString( const std::string &s );
      static std::string toString( MathFunc v );
      static MathFunc mathFuncFromString( const std::string &s );

      static std::set<ModSrc> allModSrc();
      static std::set<ModDest> allModDest();
      static std::set<MathFunc> allMathFunc();

      static double calc( MathFunc f, double v );

   protected:

   private:
      std::vector<ModSlot *> m_ModSlots;
   };
}

#endif

