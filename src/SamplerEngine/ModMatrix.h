#ifndef __MODMATRIX_H__
#define __MODMATRIX_H__

#include "JuceHeader.h"

//==============================================================================
namespace SamplerEngine
{
   class ModMatrix
   {
   public:
      ModMatrix();
      ~ModMatrix();

      static ModMatrix *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *toXml() const;

   protected:

   private:
   };
}
#endif

