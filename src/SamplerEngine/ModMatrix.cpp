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
      { ModDest_None,            ModDestInfo( ModDest_None,                      0.0,          1.0,        0.1, 0.0,    "",       "" ) },
      { ModDest_FilterCutoff,    ModDestInfo( ModDest_FilterCutoff,             -8.0,          8.0, 1.0 / 12.0, 0.0, "oct", "{:.2f}" ) },
      { ModDest_FilterResonance, ModDestInfo( ModDest_FilterResonance,        -100.0,        100.0,        0.1, 0.0,   "%", "{:.1f}" ) },
      { ModDest_Pitch,           ModDestInfo( ModDest_Pitch,             -8.0 * 12.0,   8.0 * 12.0, 1.0 / 10.0, 0.0,  "st", "{:.1f}" ) },
      { ModDest_Pan,             ModDestInfo{ ModDest_Pan,                    -100.0,        100.0,        0.1, 0.0,   "%", "{:.1f}" } },
      { ModDest_Amp,             ModDestInfo{ ModDest_Amp,                    -100.0,        100.0,        0.1, 0.0,   "%", "{:.1f}" } }

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


ModMatrix::ModSlot::ModSlot( ModMatrix::ModSrc src, ModMatrix::ModSrc mod, ModMatrix::ModDest dest, ModMatrix::MathFunc func, double amt, bool enabled ) :
   m_Enabled( enabled ),
   m_Src( src ),
   m_Mod( mod ),
   m_Dest( dest ),
   m_Func( func ),
   m_Amt( amt )
{
}


ModMatrix::ModSlot::ModSlot( const ModSlot &d ) :
   m_Enabled( d.m_Enabled ),
   m_Src( d.m_Src ),
   m_Mod( d.m_Mod ),
   m_Dest( d.m_Dest ),
   m_Func( d.m_Func ),
   m_Amt( d.m_Amt )
{
}


ModMatrix::ModSlot::ModSlot() :
   m_Enabled( false ),
   m_Src( ModMatrix::ModSrc_None ),
   m_Mod( ModMatrix::ModSrc_None ),
   m_Dest( ModMatrix::ModDest_None ),
   m_Func( ModMatrix::MathFunc_X ),
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


ModMatrix::MathFunc ModMatrix::ModSlot::getMathFunc() const
{
   return( m_Func );
}


void ModMatrix::ModSlot::setMathFunc( MathFunc f )
{
   m_Func = f;
}


ModMatrix::ModSrc ModMatrix::ModSlot::getMod() const
{
   return( m_Mod );
}


void ModMatrix::ModSlot::setMod( ModSrc mod )
{
   m_Mod = mod;
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
      if( tagName == "mod" )
      {
         pModSlot->m_Mod = ModMatrix::modSrcFromString( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "dest" )
      {
         pModSlot->m_Dest = ModMatrix::modDestFromString( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "mathfunc" )
      {
         pModSlot->m_Func = ModMatrix::mathFuncFromString(( pChild->getChildElement( 0 )->getText().toStdString() ) );
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

   juce::XmlElement *peMod = new juce::XmlElement( "mod" );
   peMod->addTextElement( ModMatrix::toString( m_Mod ) );
   pe->addChildElement( peMod );

   juce::XmlElement *peDest = new juce::XmlElement( "dest" );
   peDest->addTextElement( ModMatrix::toString( m_Dest ) );
   pe->addChildElement( peDest );

   juce::XmlElement *peFunc = new juce::XmlElement( "mathfunc" );
   peFunc->addTextElement( ModMatrix::toString( m_Func ) );
   pe->addChildElement( peFunc );

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
      ModSrc_AbsNote,
      ModSrc_RelNote,
      ModSrc_LFO1,
      ModSrc_LFO2,
      ModSrc_LFO3,
      ModSrc_ModWheel,
      ModSrc_RandomUnipolar,
      ModSrc_RandomBipolar,
      ModSrc_None
   } ) );
}


std::set<ModMatrix::ModDest> ModMatrix::allModDest()
{
   return( std::set<ModDest>( {
      ModDest_FilterCutoff,
      ModDest_FilterResonance,
      ModDest_Pitch,
      ModDest_Pan,
      ModDest_Amp,
      ModDest_None
   } ) );
}


double ModMatrix::calc( MathFunc f, double v )
{
   if( f == ModMatrix::MathFunc_OneMinusX )
      v = 1.0 - v;
   else
   if( f == ModMatrix::MathFunc_BiPolar2UniPolar )
      v = ( v + 1.0 ) / 2.0;
   else
   if( f == ModMatrix::MathFunc_UniPolar2BiPolar )
      v = ( v * 2.0 ) - 1.0;
   else
   if( f == ModMatrix::MathFunc_Squared )
      v = v * v;
   else
   if( f == MathFunc_Cubed )
      v = v * v * v;
   else
   if( f == MathFunc_Abs )
      v = v < 0 ? -v : v;
   else
   if( f == MathFunc_Neg )
      v = -v;

   return( v );
}


std::string ModMatrix::toString( MathFunc v )
{
   switch( v )
   {
      case MathFunc_X:
         return( "x" );
      case MathFunc_OneMinusX:
         return( "1-x" );
      case MathFunc_BiPolar2UniPolar:
         return( "+-->+" );
      case MathFunc_UniPolar2BiPolar:
         return( "+->+-" );
      case MathFunc_Squared:
         return( "x^2" );
      case MathFunc_Cubed:
         return( "x^3" );
      case MathFunc_Abs:
         return( "abs" );
      case MathFunc_Neg:
         return( "-x" );

      default:
         return( "x" );
   }
}


ModMatrix::MathFunc ModMatrix::mathFuncFromString( const std::string &s )
{
   if( util::trim( util::toLower( s ) ) == "x" )
      return( MathFunc_X );
   else
   if( util::trim( util::toLower( s ) ) == "1-x" )
      return( MathFunc_OneMinusX );
   else
   if( util::trim( util::toLower( s ) ) == "+-->+" )
      return( MathFunc_BiPolar2UniPolar );
   else
   if( util::trim( util::toLower( s ) ) == "+->+-" )
      return( MathFunc_UniPolar2BiPolar );
   else
   if( util::trim( util::toLower( s ) ) == "x^2" )
      return( MathFunc_Squared );
   else
   if( util::trim( util::toLower( s ) ) == "x^3" )
      return( MathFunc_Cubed );
   else
   if( util::trim( util::toLower( s ) ) == "abs" )
      return( MathFunc_Abs );
   else
   if( util::trim( util::toLower( s ) ) == "-x" )
      return( MathFunc_Neg );
   else
      return( MathFunc_X );
}


std::set<ModMatrix::MathFunc> ModMatrix::allMathFunc()
{
   return( std::set<MathFunc>( {
      MathFunc_X,
      MathFunc_OneMinusX,
      MathFunc_BiPolar2UniPolar,
      MathFunc_UniPolar2BiPolar,
      MathFunc_Squared,
      MathFunc_Cubed,
      MathFunc_Abs,
      MathFunc_Neg
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
      case( ModSrc_LFO1 ):
         return( "LFO1" );
      case( ModSrc_LFO2 ):
         return( "LFO2" );
      case( ModSrc_LFO3 ):
         return( "LFO3" );
      case( ModSrc_ModWheel ):
         return( "ModWheel" );
      case( ModSrc_Velocity ):
         return( "Velocity" );
      case( ModSrc_AbsNote ):
         return( "AbsNote" );
      case( ModSrc_RelNote ):
         return( "RelNote" );
      case( ModSrc_RandomUnipolar ):
         return( "Random+" );
      case( ModSrc_RandomBipolar ):
         return( "Random+-" );
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
      case( ModDest_Pitch ):
         return( "Pitch" );
      case( ModDest_Pan ):
         return( "Pan" );
      case( ModDest_Amp ):
         return( "Amp" );
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
   if( util::trim( util::toLower( s ) ) == "pitch" )
      return( ModDest_Pitch );
   else
   if( util::trim( util::toLower( s ) ) == "pan" )
      return( ModDest_Pan );
   else
   if( util::trim( util::toLower( s ) ) == "amp" )
      return( ModDest_Amp );
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
   if( util::trim( util::toLower( s ) ) == "lfo1" )
      return( ModSrc_LFO1 );
   else
   if( util::trim( util::toLower( s ) ) == "lfo2" )
      return( ModSrc_LFO2 );
   else
   if( util::trim( util::toLower( s ) ) == "lfo3" )
      return( ModSrc_LFO3 );
   else
   if( util::trim( util::toLower( s ) ) == "modwheel" )
      return( ModSrc_ModWheel );
   else
   if( util::trim( util::toLower( s ) ) == "velocity" )
      return( ModSrc_Velocity );
   else
   if( util::trim( util::toLower( s ) ) == "absnote" )
      return( ModSrc_AbsNote );
   else
   if( util::trim( util::toLower( s ) ) == "relnote" )
      return( ModSrc_RelNote );
   else
   if( util::trim( util::toLower( s ) ) == "random+" )
      return( ModSrc_RandomUnipolar );
   if( util::trim( util::toLower( s ) ) == "random+-" )
      return( ModSrc_RandomBipolar );
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
