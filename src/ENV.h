#pragma once

#include "JuceHeader.h"
#include <set>

//==============================================================================
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

   void setAttack( float a );
   float getAttack() const;
   void setDecay( float d );
   float getDecay() const;
   void setSustain( float s );
   float getSustain() const;
   void setRelease( float r );
   float getRelease() const;

   double getValue() const;

   bool hasEnded() const;

   static double paramToDuration( double p );

   juce::XmlElement *getStateInformation() const;

   void noteOn();
   void noteOff();

   void step( double s );
   
protected:

private:
   double m_Value;
   State m_State;
   float m_Attack;
   float m_Decay;
   float m_Sustain;
   float m_Release;
};
