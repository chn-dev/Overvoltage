#ifndef __PART_H__
#define __PART_H__

#include <list>
#include "Sample.h"

//==============================================================================
class Part
{
public:
   Part( int partNum );
   ~Part();

   std::list<Sample *> &samples();
   const std::list<Sample *> &constSamples() const;

   int getPartNum() const;
   void deleteSample( Sample *pSample );

   static Part *fromXml( const juce::XmlElement *pe );
   juce::XmlElement *getStateInformation() const;

private:
   int m_PartNum;
   std::list<Sample *> m_Samples;
};

#endif
