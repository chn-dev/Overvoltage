#include <math.h>
#include <util.h>

#include "LFO.h"

using namespace SamplerEngine;

LFO::LFO() :
   m_Value( 0.0 ),
   m_Period( 0.0 ),
   m_Waveform( LFO::Waveform_Triangle ),
   m_Frequency( 1.0 ),
   m_SyncEnabled( false ),
   m_SyncBeats( 4.0 )
{
}


LFO::LFO( const LFO &d ) :
   m_Value( 0.0 ),
   m_Period( 0.0 ),
   m_Waveform( d.m_Waveform ),
   m_Frequency( d.m_Frequency ),
   m_SyncEnabled( d.m_SyncEnabled ),
   m_SyncBeats( d.m_SyncBeats )
{
}


LFO::~LFO()
{
}


void LFO::getSettings( const LFO &d )
{
   m_Waveform = d.m_Waveform;
   m_Frequency = d.m_Frequency;
   m_SyncEnabled = d.m_SyncEnabled;
   m_SyncBeats = d.m_SyncBeats;
}


double LFO::getValue() const
{
   return( m_Value );
}


LFO::Waveform LFO::getWaveform() const
{
   return( m_Waveform );
}


void LFO::setWaveform( LFO::Waveform wf )
{
   m_Waveform = wf;
}


double LFO::getFrequency() const
{
   return( m_Frequency );
}


void LFO::setFrequency( double f )
{
   m_Frequency = f;
}


void LFO::setSyncEnabled( bool e )
{
   m_SyncEnabled = e;
}


bool LFO::getSyncEnabled() const
{
   return( m_SyncEnabled );
}


void LFO::setSyncBeats( double beats )
{
   m_SyncBeats = beats;
}


double LFO::getSyncBeats() const
{
   return( m_SyncBeats );
}


void LFO::noteOn()
{
   m_Period = 0.0;
}


void LFO::noteOff()
{
}


std::set<LFO::Waveform> LFO::allWaveforms()
{
   return( std::set<Waveform>( {
      Waveform_Sine,
      Waveform_Triangle,
      Waveform_Pulse,
      Waveform_Rectangle,
      Waveform_Sawtooth,
      Waveform_Random,
      Waveform_Custom } ) );
}


std::string LFO::toString( Waveform wf )
{
   switch( wf )
   {
      case Waveform_Sine:
         return( "Sine" );
      case Waveform_Triangle:
         return( "Triangle" );
      case Waveform_Pulse:
         return( "Pulse" );
      case Waveform_Rectangle:
         return( "Rectangle" );
      case Waveform_Sawtooth:
         return( "Sawtooth" );
      case Waveform_Random:
         return( "Random" );
      case Waveform_Custom:
         return( "Custom" );
      case Waveform_None:
         return( "None" );
      default:
         return( "None" );
   }
}


LFO::Waveform LFO::waveformFromString( const std::string &wf )
{
   if( util::toLower( util::trim( wf ) ) == "sine" )
      return( Waveform_Sine );
   else
   if( util::toLower( util::trim( wf ) ) == "triangle" )
      return( Waveform_Triangle );
   else
   if( util::toLower( util::trim( wf ) ) == "pulse" )
      return( Waveform_Pulse );
   else
   if( util::toLower( util::trim( wf ) ) == "rectangle" )
      return( Waveform_Rectangle );
   else
   if( util::toLower( util::trim( wf ) ) == "sawtooth" )
      return( Waveform_Sawtooth );
   else
   if( util::toLower( util::trim( wf ) ) == "random" )
      return( Waveform_Random );
   else
   if( util::toLower( util::trim( wf ) ) == "custom" )
      return( Waveform_Custom );
   else
      return( Waveform_None );
}


void LFO::step( double s, double bpm )
{
   double freq = m_Frequency;
   if( m_SyncEnabled )
   {
      freq = 60.0 / ( bpm * m_SyncBeats );
   }

   m_Period += ( s * freq );
   bool step = floor( m_Period ) > 0.0;
   m_Period -= floor( m_Period );

   if( m_Waveform == Waveform_Sine )
      m_Value = sin( 2.0 * M_PI * m_Period );
   else
   if( m_Waveform == Waveform_Triangle )
   {
      if( m_Period <= 0.25 )
      {
         m_Value = 4.0 * m_Period;
      } else
      if( m_Period <= 0.75 )
      {
         m_Value = 1.0 - ( 4.0 * ( m_Period - 0.25 ) );
      } else
      if( m_Period <= 1.0 )
      {
         m_Value = -1.0 + ( 4.0 * ( m_Period - 0.25 ) );
      }
   } else
   if( m_Waveform == Waveform_Pulse )
      m_Value = m_Period < 0.5 ? 0.0 : 1.0;
   else
   if( m_Waveform == Waveform_Rectangle )
      m_Value = m_Period < 0.5 ? -1.0 : 1.0;
   else
   if( m_Waveform == Waveform_Sawtooth )
      m_Value = 1.0 - ( 2.0 * m_Period );
   else
   if( m_Waveform == Waveform_Random )
   {
      if( step )
      {
         m_Value = util::randomValue( -1.0, 1.0 );
      }
   } else
   if( m_Waveform == Waveform_Custom )
   {
   }
}


LFO *LFO::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "lfo" )
      return( nullptr );

   LFO *pLFO = new LFO();

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "waveform" )
      {
         pLFO->m_Waveform = waveformFromString( util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) );
      } else
      if( tagName == "frequency" )
      {
         pLFO->m_Frequency = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "syncenabled" )
      {
         pLFO->m_SyncEnabled = util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) == "true";
      } else
      if( tagName == "syncbeats" )
      {
         pLFO->m_SyncBeats = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      }
   }

   return( pLFO );
}


juce::XmlElement *LFO::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "lfo" );

   juce::XmlElement *peWaveform = new juce::XmlElement( "waveform" );
   peWaveform->addTextElement( stdformat( "{}", toString( m_Waveform ) ) );
   pe->addChildElement( peWaveform );

   juce::XmlElement *peFrequency = new juce::XmlElement( "frequency" );
   peFrequency->addTextElement( stdformat( "{}", m_Frequency ) );
   pe->addChildElement( peFrequency );

   juce::XmlElement *peSyncEnabled = new juce::XmlElement( "syncenabled" );
   peSyncEnabled->addTextElement( m_SyncEnabled ? "true" : "false" );
   pe->addChildElement( peSyncEnabled );

   juce::XmlElement *peSyncBeats = new juce::XmlElement( "syncbeats" );
   peSyncBeats->addTextElement( stdformat( "{:.2f}", m_SyncBeats ) );
   pe->addChildElement( peSyncBeats );

   return( pe );
}
