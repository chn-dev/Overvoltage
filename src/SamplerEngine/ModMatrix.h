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
         ModSrc_Velocity
      };

      enum ModDest
      {
         ModDest_None = 1,
         ModDest_FilterCutoff,
         ModDest_FilterResonance
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
         ModSlot( ModSrc src, ModDest dest, double amt, bool enabled );
         ModSlot( const ModSlot &d );
         ModSlot();
         ~ModSlot();

         static ModSlot *fromXml( const juce::XmlElement *pe );
         juce::XmlElement *toXml() const;

         ModSrc getSrc() const;
         void setSrc( ModSrc src );
         ModDest getDest() const;
         void setDest( ModDest dest );
         double getAmount() const;
         void setAmount( double amount );
         bool isEnabled() const;
         void setEnabled( bool e );

      private:
         bool m_Enabled;
         ModSrc m_Src;
         ModDest m_Dest;
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

      static std::set<ModSrc> allModSrc();
      static std::set<ModDest> allModDest();

   protected:

   private:
      std::vector<ModSlot *> m_ModSlots;
   };
}
#endif

