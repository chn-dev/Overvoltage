/*----------------------------------------------------------------------------*/
/*!
\file ENV.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements an envelope ENV
*/
/*----------------------------------------------------------------------------*/
#include <string>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

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
ENV *ENV::fromXml( xmlNode *pe )
{
   if( std::string( (char*)pe->name ) != "envelope" )
      return( nullptr );

   ENV *pENV = new ENV();

   for( xmlNode *pChild = pe->children; pChild; pChild = pChild->next )
   {
      std::string tagName = std::string( (char*)pChild->name );

      if( tagName == "attack" )
      {
         pENV->m_Attack = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "decay" )
      {
         pENV->m_Decay = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "sustain" )
      {
         pENV->m_Sustain = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "release" )
      {
         pENV->m_Release = std::stof( std::string( (char*)pChild->children->content ) );
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
xmlNode *ENV::toXml() const
{
   xmlNode *pe = xmlNewNode( nullptr, (xmlChar *)"envelope" );

   xmlNode *peAttack = xmlNewNode( nullptr, (xmlChar *)"attack" );
   xmlAddChild( peAttack, xmlNewText( (xmlChar *)stdformat( "{}", m_Attack ).c_str() ) );
   xmlAddChild( pe, peAttack );

   xmlNode *peDecay = xmlNewNode( nullptr, (xmlChar *)"decay" );
   xmlAddChild( peDecay, xmlNewText( (xmlChar *)stdformat( "{}", m_Decay ).c_str() ) );
   xmlAddChild( pe, peDecay );

   xmlNode *peSustain = xmlNewNode( nullptr, (xmlChar *)"sustain" );
   xmlAddChild( peSustain, xmlNewText( (xmlChar *)stdformat( "{}", m_Sustain ).c_str() ) );
   xmlAddChild( pe, peSustain );

   xmlNode *peRelease = xmlNewNode( nullptr, (xmlChar *)"release" );
   xmlAddChild( peRelease, xmlNewText( (xmlChar *)stdformat( "{}", m_Release ).c_str() ) );
   xmlAddChild( pe, peRelease );

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

