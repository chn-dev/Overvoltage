#ifndef __MODMATRIX_H__
#define __MODMATRIX_H__

#include "JuceHeader.h"

//==============================================================================
namespace SamplerEngine
{
   class ModMatrix
   {
      enum ModSrc
      {
         ModSrc_None = 1,
         ModSrc_AEG,
         ModSrc_EG2
      };

      enum ModDest
      {
         ModDest_None = 1,
         ModDest_FilterCutoff,
         ModDest_FilterResonance
      };

   public:
      class ModSlot
      {
      public:
         ModSlot( ModSrc src, ModDest dest, double amt );
         ModSlot( const ModSlot &d );
         ModSlot();
         ~ModSlot();

         static ModSlot *fromXml( const juce::XmlElement *pe );
         juce::XmlElement *toXml() const;

      private:
         ModSrc m_Src;
         ModDest m_Dest;
         double m_Amt;
      };

      ModMatrix();
      ~ModMatrix();

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
      std::vector<ModSlot> m_ModSlots;
   };
}
#endif

