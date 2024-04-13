#include <string>

#include "ModMatrix.h"

#include <util.h>

using namespace SamplerEngine;


ModMatrix::ModSlot::ModSlot( ModMatrix::ModSrc src, ModMatrix::ModDest dest, double amt ) :
   m_Src( src ),
   m_Dest( dest ),
   m_Amt( amt )
{
}


ModMatrix::ModSlot::ModSlot( const ModSlot &d ) :
   m_Src( d.m_Src ),
   m_Dest( d.m_Dest ),
   m_Amt( d.m_Amt )
{
}


ModMatrix::ModSlot::ModSlot() :
   m_Src( ModMatrix::ModSrc_None ),
   m_Dest( ModMatrix::ModDest_None ),
   m_Amt( 0.0 )
{
}


ModMatrix::ModSlot::~ModSlot()
{
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

   return( pe );
}


ModMatrix::ModMatrix()
{
   m_ModSlots.resize( 6 );
}


ModMatrix::~ModMatrix()
{
}


ModMatrix *ModMatrix::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "modmatrix" )
      return( nullptr );

   ModMatrix *pModMatrix = new ModMatrix();
   pModMatrix->m_ModSlots.clear();

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "modslot" )
      {
         ModSlot *pModSlot = ModSlot::fromXml( pChild );
         if( pModSlot )
         {
            pModMatrix->m_ModSlots.push_back( *pModSlot );
            delete pModSlot;
         }
      }
   }

   return( pModMatrix );
}


juce::XmlElement *ModMatrix::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "modmatrix" );

   size_t s = m_ModSlots.size();

   for( int i = 0; i < m_ModSlots.size(); i++ )
   {
      juce::XmlElement *peModSlot = m_ModSlots[i].toXml();
      pe->addChildElement( peModSlot );
   }

   return( pe );
}


std::set<ModMatrix::ModSrc> ModMatrix::allModSrc()
{
   return( std::set<ModSrc>( {
      ModSrc_AEG,
      ModSrc_EG2
   } ) );
}


std::set<ModMatrix::ModDest> ModMatrix::allModDest()
{
   return( std::set<ModDest>( {
      ModDest_FilterCutoff,
      ModDest_FilterResonance
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
      return( ModSrc_None );
}
