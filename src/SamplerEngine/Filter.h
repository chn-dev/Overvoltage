#ifndef __FILTER_H__
#define __FILTER_H__

#include "JuceHeader.h"

//==============================================================================
namespace SamplerEngine
{
   class Filter
   {
   public:
      Filter();
      ~Filter();

      double getCutoff() const;
      double getResonance() const;
      void setCutoff( double v );
      void setResonance( double v );

      static Filter *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *toXml() const;

   protected:

   private:
      double m_Cutoff;
      double m_Resonance;
   };
}
#endif

