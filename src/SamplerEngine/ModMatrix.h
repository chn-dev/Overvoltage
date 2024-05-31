#ifndef __MODMATRIX_H__
#define __MODMATRIX_H__

#include "JuceHeader.h"

//==============================================================================
namespace SamplerEngine
{
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
         ModSrc_Gate
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

      class ModSlot
      {
      public:
         ModSlot( ModSrc src, ModSrc mod, ModDest dest, MathFunc func, double amt, bool enabled );
         ModSlot( const ModSlot &d );
         ModSlot();
         ~ModSlot();

         static ModSlot *fromXml( const juce::XmlElement *pe );
         juce::XmlElement *toXml() const;

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

      ModMatrix();
      ~ModMatrix();

      size_t numSlots() const;
      ModSlot *getSlot( size_t n ) const;

      static ModMatrix *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *toXml() const;

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

