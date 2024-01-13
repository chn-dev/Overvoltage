#ifndef __PART_H__
#define __PART_H__

#include <list>
#include "Sample.h"

//==============================================================================
namespace Overvoltage
{
   class Part
   {
   public:
      Part( size_t partNum );
      ~Part();

      std::list<Sample *> &samples();
      const std::list<Sample *> &constSamples() const;

      size_t getPartNum() const;
      void deleteSample( Sample *pSample );

      static Part *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *getStateInformation() const;

   private:
      size_t m_PartNum;
      std::list<Sample *> m_Samples;
   };
}

#endif
