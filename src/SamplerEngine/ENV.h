#ifndef __ENV_H__
#define __ENV_H__

#include "JuceHeader.h"
#include <set>

//==============================================================================
namespace Overvoltage
{
   class ENV
   {
   public:
      ENV();
      ~ENV();

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

      juce::XmlElement *getStateInformation() const;

      void noteOn();
      void noteOff();

      void step( double s );

      static ENV *fromXml( const juce::XmlElement *pe );

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

