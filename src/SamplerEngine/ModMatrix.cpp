#include <string>

#include "ModMatrix.h"

#include <util.h>

using namespace SamplerEngine;


ModMatrix::ModDestInfo::ModDestInfo( ModDest md ) :
   m_Type( md ),
   m_Min( infoMap()[md].getMin() ),
   m_Max( infoMap()[md].getMax() ),
   m_Step( infoMap()[md].getStep() ),
   m_Default( infoMap()[md].getDefaultValue() ),
   m_Unit( infoMap()[md].getUnit() ),
   m_Format( infoMap()[md].getFormat() )
{
}


ModMatrix::ModDestInfo::ModDestInfo(
   ModDest md,
   double minVal, double maxVal, double step, double defaultVal,
   std::string unit, std::string format ) :
   m_Type( md ),
   m_Min( minVal ), m_Max( maxVal ), m_Step( step),
   m_Default( defaultVal ),
   m_Unit( unit ), m_Format( format )
{
}


ModMatrix::ModDestInfo::ModDestInfo() :
   m_Type( ModDest_None )
{
}


ModMatrix::ModDestInfo::~ModDestInfo()
{
}


std::map<ModMatrix::ModDest, ModMatrix::ModDestInfo> &ModMatrix::ModDestInfo::infoMap()
{
   static std::map<ModDest, ModDestInfo> theMap =
   {
      { ModDest_None,            ModDestInfo( ModDest_None,               0.0,   1.0,        0.1, 0.0, "",    ""       ) },
      { ModDest_FilterCutoff,    ModDestInfo( ModDest_FilterCutoff,      -8.0,   8.0, 1.0 / 12.0, 0.0, "oct", "{:.2f}" ) },
      { ModDest_FilterResonance, ModDestInfo( ModDest_FilterResonance, -100.0, 100.0,        0.1, 0.0, "%",   "{:.2f}" ) }
   };

   return( theMap );
}



double ModMatrix::ModDestInfo::getMin() const
{
   return( m_Min );
}


double ModMatrix::ModDestInfo::getMax() const
{
   return( m_Max );
}


double ModMatrix::ModDestInfo::getStep() const
{
   return( m_Step );
}


std::string ModMatrix::ModDestInfo::getFormat() const
{
   return( m_Format );
}


double ModMatrix::ModDestInfo::getDefaultValue() const
{
   return( m_Default );
}


ModMatrix::ModDest ModMatrix::ModDestInfo::getType() const
{
   return( m_Type );
}


std::string ModMatrix::ModDestInfo::getUnit() const
{
   return( m_Unit );
}


ModMatrix::ModSlot::ModSlot( ModMatrix::ModSrc src, ModMatrix::ModDest dest, double amt, bool enabled ) :
   m_Enabled( enabled ),
   m_Src( src ),
   m_Dest( dest ),
   m_Amt( amt )
{
}


ModMatrix::ModSlot::ModSlot( const ModSlot &d ) :
   m_Enabled( d.m_Enabled ),
   m_Src( d.m_Src ),
   m_Dest( d.m_Dest ),
   m_Amt( d.m_Amt )
{
}


ModMatrix::ModSlot::ModSlot() :
   m_Enabled( false ),
   m_Src( ModMatrix::ModSrc_None ),
   m_Dest( ModMatrix::ModDest_None ),
   m_Amt( 0.0 )
{
}


ModMatrix::ModSlot::~ModSlot()
{
}


void ModMatrix::ModSlot::setDest( ModDest dest )
{
   m_Dest = dest;
}


ModMatrix::ModSrc ModMatrix::ModSlot::getSrc() const
{
   return( m_Src );
}


void ModMatrix::ModSlot::setSrc( ModSrc src )
{
   m_Src = src;
}


ModMatrix::ModDest ModMatrix::ModSlot::getDest() const
{
   return( m_Dest );
}


double ModMatrix::ModSlot::getAmount() const
{
   return( m_Amt );
}


void ModMatrix::ModSlot::setAmount( double amount )
{
   m_Amt = amount;
}


bool ModMatrix::ModSlot::isEnabled() const
{
   return( m_Enabled );
}


void ModMatrix::ModSlot::setEnabled( bool e )
{
   m_Enabled = e;
}


ModMatrix::ModSlot *ModMatrix::ModSlot::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "modslot" )
      return( nullptr );

   ModSlot *pModSlot = new ModSlot();

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "src" )
      {
         pModSlot->m_Src = ModMatrix::modSrcFromString( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "dest" )
      {
         pModSlot->m_Dest = ModMatrix::modDestFromString( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "amt" )
      {
         pModSlot->m_Amt = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "enabled" )
      {
         pModSlot->m_Enabled = ( util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) == "true" );
      }
   }

   return( pModSlot );
}


juce::XmlElement *ModMatrix::ModSlot::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "modslot" );

   juce::XmlElement *peSrc = new juce::XmlElement( "src" );
   peSrc->addTextElement( ModMatrix::toString( m_Src ) );
   pe->addChildElement( peSrc );

   juce::XmlElement *peDest = new juce::XmlElement( "dest" );
   peDest->addTextElement( ModMatrix::toString( m_Dest ) );
   pe->addChildElement( peDest );

   juce::XmlElement *peAmt = new juce::XmlElement( "amt" );
   peAmt->addTextElement( stdformat( "{}", m_Amt ) );
   pe->addChildElement( peAmt );

   juce::XmlElement *peEnabled = new juce::XmlElement( "enabled" );
   peEnabled->addTextElement( m_Enabled ? "true" : "false" );
   pe->addChildElement( peEnabled );

   return( pe );
}


ModMatrix::ModMatrix()
{
}


ModMatrix::~ModMatrix()
{
   for( size_t i = 0; i < m_ModSlots.size(); i++ )
   {
      delete m_ModSlots[i];
   }
   m_ModSlots.clear();
}


ModMatrix *ModMatrix::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "modmatrix" )
      return( nullptr );

   ModMatrix *pModMatrix = new ModMatrix();
   pModMatrix->m_ModSlots.clear();

   for( int i = 0; /*pe->getChildElement( i ) && */i < 5; i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      if( pChild )
      {
         std::string tagName = pChild->getTagName().toStdString();

         if( tagName == "modslot" )
         {
            ModSlot *pModSlot = ModSlot::fromXml( pChild );
            if( pModSlot )
            {
               pModMatrix->m_ModSlots.push_back( pModSlot );
            }
         }
      } else
      {
         pModMatrix->m_ModSlots.push_back( new ModSlot() );
      }
   }

   return( pModMatrix );
}


juce::XmlElement *ModMatrix::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "modmatrix" );

   for( size_t i = 0; i < m_ModSlots.size() && i < 5; i++ )
   {
      juce::XmlElement *peModSlot = m_ModSlots[i]->toXml();
      pe->addChildElement( peModSlot );
   }

   return( pe );
}


std::set<ModMatrix::ModSrc> ModMatrix::allModSrc()
{
   return( std::set<ModSrc> ( {
      ModSrc_AEG,
      ModSrc_EG2,
      ModSrc_Velocity,
      ModSrc_None
   } ) );
}


std::set<ModMatrix::ModDest> ModMatrix::allModDest()
{
   return( std::set<ModDest>( {
      ModDest_FilterCutoff,
      ModDest_FilterResonance,
      ModDest_None
   } ) );
}


std::string ModMatrix::toString( ModSrc v )
{
   switch( v )
   {
      case( ModSrc_AEG ):
         return( "AEG" );
      case( ModSrc_EG2 ):
         return( "EG2" );
      case( ModSrc_Velocity ):
         return( "Velocity" );
      case( ModSrc_None ):
         return( "None" );
      default:
         return( "None" );
   }
}


std::string ModMatrix::toString( ModDest v )
{
   switch( v )
   {
      case( ModDest_FilterCutoff ):
         return( "FilterCutoff" );
      case( ModDest_FilterResonance ):
         return( "FilterResonance" );
      case( ModDest_None ):
         return( "None" );
      default:
         return( "None" );
   }
}


ModMatrix::ModDest ModMatrix::modDestFromString( const std::string &s )
{
   if( util::trim( util::toLower( s ) ) == "filtercutoff" )
      return( ModDest_FilterCutoff );
   else
   if( util::trim( util::toLower( s ) ) == "filterresonance" )
      return( ModDest_FilterResonance );
   else
      return( ModDest_None );
}


ModMatrix::ModSrc ModMatrix::modSrcFromString( const std::string &s )
{
   if( util::trim( util::toLower( s ) ) == "aeg" )
      return( ModSrc_AEG );
   else
   if( util::trim( util::toLower( s ) ) == "eg2" )
      return( ModSrc_EG2 );
   else
   if( util::trim( util::toLower( s ) ) == "velocity" )
      return( ModSrc_Velocity );
   else
      return( ModSrc_None );
}


size_t ModMatrix::numSlots() const
{
   return( m_ModSlots.size() );
}


ModMatrix::ModSlot *ModMatrix::getSlot( size_t n ) const
{
   if( n >= numSlots() )
      return( nullptr );
   return( m_ModSlots[n] );
}
