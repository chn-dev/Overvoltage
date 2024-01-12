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

   float getValue() const;

   bool hasEnded() const;

   static float paramToDuration( float p );

   juce::XmlElement *getStateInformation() const;

   void noteOn();
   void noteOff();

   void step( float s );

   static ENV *fromXml( const juce::XmlElement *pe );

protected:

private:
   float m_Value;
   State m_State;
   float m_Attack;
   float m_Decay;
   float m_Sustain;
   float m_Release;
};
