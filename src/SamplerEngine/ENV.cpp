/*----------------------------------------------------------------------------*/
/*!
\file ENV.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements an envelope ENV
*/
/*----------------------------------------------------------------------------*/
#include <string>
#include <math.h>

#include "ENV.h"

#include <util.h>

using namespace SamplerEngine;


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Constructor
*/
/*----------------------------------------------------------------------------*/
ENV::ENV() :
   m_Value( 0.0 ),
   m_State( StateNone ),
   m_Attack( 0.0 ),
   m_Decay( 0.0 ),
   m_Sustain( 1.0 ),
   m_Release( 0.0 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Copy the envelope settings from another ENV instance.
\param d Reference to the other ENV instance
*/
/*----------------------------------------------------------------------------*/
void ENV::getSettings( const ENV &d )
{
   m_Attack = d.m_Attack;
   m_Decay = d.m_Decay;
   m_Sustain = d.m_Sustain;
   m_Release = d.m_Release;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Destructor
*/
/*----------------------------------------------------------------------------*/
ENV::~ENV()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Reconstruct an ENV object from a previously generated XML element (see toXml()).
\param pe The XML element
\return Pointer to the ENV object or nullptr on error
*/
/*----------------------------------------------------------------------------*/
ENV *ENV::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "envelope" )
      return( nullptr );

   ENV *pENV = new ENV();

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "attack" )
      {
         pENV->m_Attack = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "decay" )
      {
         pENV->m_Decay = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "sustain" )
      {
         pENV->m_Sustain = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "release" )
      {
         pENV->m_Release = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      }
   }

   return( pENV );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Create an XML element from the ENV settings.
\return Pointer to the new XML element
*/
/*----------------------------------------------------------------------------*/
juce::XmlElement *ENV::toXml() const
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return true if the envelope has reached its end.
*/
/*----------------------------------------------------------------------------*/
bool ENV::hasEnded() const
{
   return( m_State == StateEnd );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Calculated a duration in seconds from a parameter ranging from 0 to 1.
\param p The parameter (0..1)
\return The corresponding duration in seconds
*/
/*----------------------------------------------------------------------------*/
double ENV::paramToDuration( double p )
{
   if( p < 0.0 )
      p = 0.0;
   else
   if( p > 1.0 )
      p = 1.0;

   return( 10.0 * pow( p, 2.0 ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Process the envelope.
\param s The elapsed time since the previous invocation of step()
\param bpm The current song speed in bpm
*/
/*----------------------------------------------------------------------------*/
void ENV::step( double s, double /*bpm*/ )
{
   if( m_State == StateAttack )
   {
      if( m_Attack == 0 )
      {
         m_Value = 1.0;
      } else
      {
         m_Value += s / paramToDuration( m_Attack );
      }

      if( m_Value >= 1.0 )
      {
         m_Value = 1.0;
         m_State = StateDecay;
      }
   } else
   if( m_State == StateDecay )
   {
      if( m_Decay == 0 )
      {
         m_Value = m_Sustain;
      } else
      {
         m_Value -= s / paramToDuration( m_Decay );
      }

      if( m_Value <= m_Sustain )
      {
         m_Value = m_Sustain;
         m_State = StateSustain;
      }
   } else
   if( m_State == StateSustain )
   {
      m_Value = m_Sustain;
   } else
   if( m_State == StateRelease )
   {
      if( m_Release == 0 )
      {
         m_Value = 0;
      } else
      {
         m_Value -= s / paramToDuration( m_Release );
      }

      if( m_Value <= 0.0 )
      {
         m_Value = 0.0;
         m_State = StateEnd;
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void ENV::noteOn()
{
   m_Value = 0.0;
   m_State = StateAttack;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void ENV::noteOff()
{
   if( m_State != StateEnd && m_State != StateNone )
   {
      m_State = StateRelease;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The current output value of the envelope ranging from 0 to 1.
*/
/*----------------------------------------------------------------------------*/
double ENV::getValue() const
{
   return( m_Value );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void ENV::setAttack( double a )
{
   m_Attack = a;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
double ENV::getAttack() const
{
   return( m_Attack );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void ENV::setDecay( double d )
{
   m_Decay = d;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
double ENV::getDecay() const
{
   return( m_Decay );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void ENV::setSustain( double s )
{
   m_Sustain = s;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
double ENV::getSustain() const
{
   return( m_Sustain );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void ENV::setRelease( double r )
{
   m_Release = r;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
double ENV::getRelease() const
{
   return( m_Release );
}

