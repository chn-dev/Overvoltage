#define _USE_MATH_DEFINES
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
   m_SyncBeats( 4.0 ),
   m_DelaySyncEnabled( false ),
   m_DelaySecs( 0.0 ),
   m_DelayBeats( 0.0 ),
   m_FadeInSyncEnabled( false ),
   m_FadeInSecs( 0.0 ),
   m_FadeInBeats( 0.0 ),
   m_OnceEnabled( false ),
   m_RandomPhaseEnabled( false )
{
}


LFO::LFO( const LFO &d ) :
   m_Value( 0.0 ),
   m_Period( 0.0 ),
   m_Waveform( d.m_Waveform ),
   m_Frequency( d.m_Frequency ),
   m_SyncEnabled( d.m_SyncEnabled ),
   m_SyncBeats( d.m_SyncBeats ),
   m_DelaySyncEnabled( d.m_DelaySyncEnabled ),
   m_DelaySecs( d.m_DelaySecs ),
   m_DelayBeats( d.m_DelayBeats ),
   m_FadeInSyncEnabled( d.m_DelaySyncEnabled ),
   m_FadeInSecs( d.m_DelaySecs ),
   m_FadeInBeats( d.m_DelayBeats ),
   m_OnceEnabled( d.m_OnceEnabled ),
   m_RandomPhaseEnabled( d.m_RandomPhaseEnabled )
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
   m_DelaySyncEnabled = d.m_DelaySyncEnabled;
   m_DelaySecs = d.m_DelaySecs;
   m_DelayBeats = d.m_DelayBeats;
   m_FadeInSyncEnabled = d.m_FadeInSyncEnabled;
   m_FadeInSecs = d.m_FadeInSecs;
   m_FadeInBeats = d.m_FadeInBeats;
   m_OnceEnabled = d.m_OnceEnabled;
   m_RandomPhaseEnabled = d.m_RandomPhaseEnabled;
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


void LFO::setDelaySyncEnabled( bool e )
{
   m_DelaySyncEnabled = e;
}


bool LFO::getDelaySyncEnabled() const
{
   return( m_DelaySyncEnabled );
}


void LFO::setDelayBeats( double beats )
{
   m_DelayBeats = beats;
}


double LFO::getDelayBeats() const
{
   return( m_DelayBeats );
}


void LFO::setDelaySecs( double s )
{
   m_DelaySecs = s;
}


double LFO::getDelaySecs() const
{
   return( m_DelaySecs );
}


void LFO::setFadeInSyncEnabled( bool e )
{
   m_FadeInSyncEnabled = e;
}


bool LFO::getFadeInSyncEnabled() const
{
   return( m_FadeInSyncEnabled );
}


void LFO::setFadeInBeats( double beats )
{
   m_FadeInBeats = beats;
}


double LFO::getFadeInBeats() const
{
   return( m_FadeInBeats );
}


void LFO::setFadeInSecs( double s )
{
   m_FadeInSecs = s;
}


double LFO::getFadeInSecs() const
{
   return( m_FadeInSecs );
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


void LFO::setOnceEnabled( bool e )
{
   m_OnceEnabled = e;
}


bool LFO::getOnceEnabled() const
{
   return( m_OnceEnabled );
}


void LFO::setRandomPhaseEnabled( bool e )
{
   m_RandomPhaseEnabled = e;
}


bool LFO::getRandomPhaseEnabled() const
{
   return( m_RandomPhaseEnabled );
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
      freq = 240.0 / ( bpm * m_SyncBeats );
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
         m_Value = -1.0 + ( 4.0 * ( m_Period - 0.75 ) );
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
      } else
      if( tagName == "delaysyncenabled" )
      {
         pLFO->m_DelaySyncEnabled = util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) == "true";
      } else
      if( tagName == "delaysecs" )
      {
         pLFO->m_DelaySecs = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "delaybeats" )
      {
         pLFO->m_DelayBeats = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "fadeinsyncenabled" )
      {
         pLFO->m_FadeInSyncEnabled = util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) == "true";
      } else
      if( tagName == "fadeinsecs" )
      {
         pLFO->m_FadeInSecs = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "fadeinbeats" )
      {
         pLFO->m_FadeInBeats = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "onceenabled" )
      {
         pLFO->m_OnceEnabled = util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) == "true";
      } else
      if( tagName == "randomphaseenabled" )
      {
         pLFO->m_RandomPhaseEnabled = util::toLower( util::trim( pChild->getChildElement( 0 )->getText().toStdString() ) ) == "true";
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

   juce::XmlElement *peDelaySyncEnabled = new juce::XmlElement( "delaysyncenabled" );
   peDelaySyncEnabled->addTextElement( m_DelaySyncEnabled ? "true" : "false" );
   pe->addChildElement( peDelaySyncEnabled );

   juce::XmlElement *peDelaySecs = new juce::XmlElement( "delaysecs" );
   peDelaySecs->addTextElement( stdformat( "{:.2f}", m_DelaySecs ) );
   pe->addChildElement( peDelaySecs );

   juce::XmlElement *peDelayBeats = new juce::XmlElement( "delaybeats" );
   peDelayBeats->addTextElement( stdformat( "{:.2f}", m_DelayBeats ) );
   pe->addChildElement( peDelayBeats );

   juce::XmlElement *peFadeInSyncEnabled = new juce::XmlElement( "fadeinsyncenabled" );
   peFadeInSyncEnabled->addTextElement( m_FadeInSyncEnabled ? "true" : "false" );
   pe->addChildElement( peFadeInSyncEnabled );

   juce::XmlElement *peFadeInSecs = new juce::XmlElement( "fadeinsecs" );
   peFadeInSecs->addTextElement( stdformat( "{:.2f}", m_FadeInSecs ) );
   pe->addChildElement( peFadeInSecs );

   juce::XmlElement *peFadeInBeats = new juce::XmlElement( "fadeinbeats" );
   peFadeInBeats->addTextElement( stdformat( "{:.2f}", m_FadeInBeats ) );
   pe->addChildElement( peFadeInBeats );

   juce::XmlElement *peOnceEnabled = new juce::XmlElement( "onceenabled" );
   peOnceEnabled->addTextElement( m_OnceEnabled ? "true" : "false" );
   pe->addChildElement( peOnceEnabled );

   juce::XmlElement *peRandomPhaseEnabled = new juce::XmlElement( "randomphaseenabled" );
   peRandomPhaseEnabled->addTextElement( m_RandomPhaseEnabled ? "true" : "false" );
   pe->addChildElement( peRandomPhaseEnabled );

   return( pe );
}
