#include <math.h>

#include "ENV.h"

#include "util.h"

ENV::ENV() :
   m_Attack( 0.0 ),
   m_Decay( 0.0 ),
   m_Sustain( 1.0 ),
   m_Release( 0.0 ),
   m_Value( 0.0 ),
   m_State( StateNone )
{
}


ENV::~ENV()
{
}


juce::XmlElement *ENV::getStateInformation() const
{
   juce::XmlElement *pe = new juce::XmlElement( "envelope" );

   juce::XmlElement *peAttack = new juce::XmlElement( "attack" );
   peAttack->addTextElement( stdformat( "{}", m_Attack ) );
   pe->addChildElement( peAttack );

   juce::XmlElement *peDecay = new juce::XmlElement( "decay" );
   peDecay->addTextElement( stdformat( "{}", m_Decay ) );
   pe->addChildElement( peDecay );

   juce::XmlElement *peSustain = new juce::XmlElement( "sustain" );
   peSustain->addTextElement( stdformat( "{}", m_Sustain ) );
   pe->addChildElement( peSustain );

   juce::XmlElement *peRelease = new juce::XmlElement( "release" );
   peRelease->addTextElement( stdformat( "{}", m_Release ) );
   pe->addChildElement( peRelease );

   return( pe );
}


bool ENV::hasEnded() const
{
   return( m_State == StateEnd );
}


double ENV::paramToDuration( double p )
{
   if( p < 0 )
      p = 0.0;
   else
   if( p > 1.0 )
      p = 1.0;

   return( 10.0 * pow( p, 2.0 ) );
}


void ENV::step( double s )
{
   if( m_State == StateAttack )
   {
      m_Value += s / paramToDuration( m_Attack );

      if( m_Value >= 1.0 )
      {
         m_Value = 1.0;
         m_State = StateDecay;
      }
   } else
   if( m_State == StateDecay )
   {
      m_Value -= s / paramToDuration( m_Decay );

      if( m_Value <= m_Decay )
      {
         m_Value = m_Decay;
         m_State = StateSustain;
      }
   } else
   if( m_State == StateSustain )
   {
      m_Value = m_Sustain;
   } else
   if( m_State == StateRelease )
   {
      m_Value -= s / paramToDuration( m_Release );

      if( m_Value < 0.0 )
      {
         m_Value = 0.0;
         m_State = StateEnd;
      }
   }
}


void ENV::noteOn()
{
   m_Value = 0.0;
   m_State = StateAttack;
}


void ENV::noteOff()
{
   if( m_State != StateEnd && m_State != StateNone )
   {
      m_State = StateRelease;
   }
}


double ENV::getValue() const
{
   return( m_Value );
}


void ENV::setAttack( float a )
{
   m_Attack = a;
}


float ENV::getAttack() const
{
   return( m_Attack );
}


void ENV::setDecay( float d )
{
   m_Decay = d;
}


float ENV::getDecay() const
{
   return( m_Decay );
}


void ENV::setSustain( float s )
{
   m_Sustain = s;
}


float ENV::getSustain() const
{
   return( m_Sustain );
}


void ENV::setRelease( float r )
{
   m_Release = r;
}


float ENV::getRelease() const
{
   return( m_Release );
}
