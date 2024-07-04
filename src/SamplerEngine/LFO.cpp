/*----------------------------------------------------------------------------*/
/*!
\file LFO.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements an LFO
*/
/*----------------------------------------------------------------------------*/
#define _USE_MATH_DEFINES
#include <math.h>
#include <util.h>

#include "LFO.h"

using namespace SamplerEngine;


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
Constructor
*/
/*----------------------------------------------------------------------------*/
LFO::LFO() :
   m_Value( 0.0 ),
   m_Period( 0.0 ),
   m_PeriodCount( 0 ),
   m_TimeS( 0.0 ),
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
   m_RandomPhaseEnabled( false ),
   m_Custom( { 1.0, -1.0 } ),
   m_CustomQuantizeEnabled( false ),
   m_CustomQuantize( 12 ),
   m_StartPhase( 0.0 )
{
}



/*----------------------------------------------------------------------------*/
/*! 2024-06-16
Copy constructor
*/
/*----------------------------------------------------------------------------*/
LFO::LFO( const LFO &d ) :
   m_Value( 0.0 ),
   m_Period( 0.0 ),
   m_PeriodCount( 0 ),
   m_TimeS( 0.0 ),
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
   m_RandomPhaseEnabled( d.m_RandomPhaseEnabled ),
   m_Custom( d.m_Custom ),
   m_CustomQuantizeEnabled( d.m_CustomQuantizeEnabled ),
   m_CustomQuantize( d.m_CustomQuantize ),
   m_StartPhase( d.m_StartPhase )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
Destructor
*/
/*----------------------------------------------------------------------------*/
LFO::~LFO()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
Copy LFO settings from another LFO object.
\param d
*/
/*----------------------------------------------------------------------------*/
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
   m_Custom = d.m_Custom;
   m_CustomQuantizeEnabled = d.m_CustomQuantizeEnabled;
   m_CustomQuantize = d.m_CustomQuantize;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\return The current LFO valus
*/
/*----------------------------------------------------------------------------*/
double LFO::getValue() const
{
   return( m_Value );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\return The waveform
*/
/*----------------------------------------------------------------------------*/
LFO::Waveform LFO::getWaveform() const
{
   return( m_Waveform );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
Set the waveform
\param wf
*/
/*----------------------------------------------------------------------------*/
void LFO::setWaveform( LFO::Waveform wf )
{
   m_Waveform = wf;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\param e true if the start delay time of the LFO shall be synced to the tempo
*/
/*----------------------------------------------------------------------------*/
void LFO::setDelaySyncEnabled( bool e )
{
   m_DelaySyncEnabled = e;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\return true if the start delay time of the LFO is synced to the tempo
*/
/*----------------------------------------------------------------------------*/
bool LFO::getDelaySyncEnabled() const
{
   return( m_DelaySyncEnabled );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\param beats Length of the start delay in beats
*/
/*----------------------------------------------------------------------------*/
void LFO::setDelayBeats( double beats )
{
   m_DelayBeats = beats;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\return Length of the start delay in beats
*/
/*----------------------------------------------------------------------------*/
double LFO::getDelayBeats() const
{
   return( m_DelayBeats );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\param s Length of the start delay in seconds
*/
/*----------------------------------------------------------------------------*/
void LFO::setDelaySecs( double s )
{
   m_DelaySecs = s;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\return Length of the start delay in seconds
*/
/*----------------------------------------------------------------------------*/
double LFO::getDelaySecs() const
{
   return( m_DelaySecs );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\param e true if the fade in time shall be synced to the tempo
*/
/*----------------------------------------------------------------------------*/
void LFO::setFadeInSyncEnabled( bool e )
{
   m_FadeInSyncEnabled = e;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-16
\return true if the fade in time is synced to the tempo
*/
/*----------------------------------------------------------------------------*/
bool LFO::getFadeInSyncEnabled() const
{
   return( m_FadeInSyncEnabled );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param beats The number of beats the LFO uses to fade in
*/
/*----------------------------------------------------------------------------*/
void LFO::setFadeInBeats( double beats )
{
   m_FadeInBeats = beats;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of beats the LFO uses to fade in
*/
/*----------------------------------------------------------------------------*/
double LFO::getFadeInBeats() const
{
   return( m_FadeInBeats );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param s The number of seconds the LFO uses to fade in
*/
/*----------------------------------------------------------------------------*/
void LFO::setFadeInSecs( double s )
{
   m_FadeInSecs = s;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of seconds the LFO uses to fade in
*/
/*----------------------------------------------------------------------------*/
double LFO::getFadeInSecs() const
{
   return( m_FadeInSecs );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The LFO frequency in Hz
*/
/*----------------------------------------------------------------------------*/
double LFO::getFrequency() const
{
   return( m_Frequency );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param f The LFO frequency in Hz
*/
/*----------------------------------------------------------------------------*/
void LFO::setFrequency( double f )
{
   m_Frequency = f;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param e true if the LFO rate shall be synced to the host's tempo
*/
/*----------------------------------------------------------------------------*/
void LFO::setSyncEnabled( bool e )
{
   m_SyncEnabled = e;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the LFO rate is synced to the host's tempo
*/
/*----------------------------------------------------------------------------*/
bool LFO::getSyncEnabled() const
{
   return( m_SyncEnabled );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param beats The LFO rate in beats
*/
/*----------------------------------------------------------------------------*/
void LFO::setSyncBeats( double beats )
{
   m_SyncBeats = beats;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The LFO rate in beats
*/
/*----------------------------------------------------------------------------*/
double LFO::getSyncBeats() const
{
   return( m_SyncBeats );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param e true if the LFO shall cycle only once
*/
/*----------------------------------------------------------------------------*/
void LFO::setOnceEnabled( bool e )
{
   m_OnceEnabled = e;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the LFO cycles only once
*/
/*----------------------------------------------------------------------------*/
bool LFO::getOnceEnabled() const
{
   return( m_OnceEnabled );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param e true if the LFO start phase shall be randomized
*/
/*----------------------------------------------------------------------------*/
void LFO::setRandomPhaseEnabled( bool e )
{
   m_RandomPhaseEnabled = e;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the LFO start phase is randomized
*/
/*----------------------------------------------------------------------------*/
bool LFO::getRandomPhaseEnabled() const
{
   return( m_RandomPhaseEnabled );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The custom waveform
*/
/*----------------------------------------------------------------------------*/
std::vector<double> LFO::getCustom() const
{
   return( m_Custom );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Reference to the custom waveform
*/
/*----------------------------------------------------------------------------*/
std::vector<double> &LFO::getCustomRef()
{
   return( m_Custom );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param e true if the custum waveform shall be quantized
*/
/*----------------------------------------------------------------------------*/
void LFO::setCustomQuantizeEnabled( bool e )
{
   m_CustomQuantizeEnabled = e;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the custum waveform is quantized
*/
/*----------------------------------------------------------------------------*/
bool LFO::getCustomQuantizeEnabled() const
{
   return( m_CustomQuantizeEnabled );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param q The number of values the custom waveform shall be quantized to
*/
/*----------------------------------------------------------------------------*/
void LFO::setCustomQuantize( size_t q )
{
   m_CustomQuantize = q;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of values the custom waveform is being quantized to
*/
/*----------------------------------------------------------------------------*/
size_t LFO::getCustomQuantize() const
{
   return( m_CustomQuantize );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param v The custom waveform
*/
/*----------------------------------------------------------------------------*/
void LFO::setCustom( std::vector<double> v )
{
   m_Custom = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the LFO is to be started.
*/
/*----------------------------------------------------------------------------*/
void LFO::noteOn()
{
   if( m_RandomPhaseEnabled )
   {
      m_StartPhase = util::randomValue( 0.0, 1.0 );
      m_StartPhase -= floor( m_StartPhase );
   } else
   {
      m_StartPhase = 0.0;
   }

   m_Period = 0.0;
   m_PeriodCount = 0;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Called when the LFO is to be stopped.
*/
/*----------------------------------------------------------------------------*/
void LFO::noteOff()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A set of of all possible LFO waveforms
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param An LFO waveform type
\return The string representation of the waveform type
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param wf The string representation of an LFO waveform type
\return The waveform type
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Retrieve a value of the custom waveform.
\param i The index of the value to be retrieved
\return The value
*/
/*----------------------------------------------------------------------------*/
double LFO::getCustomValue( size_t i ) const
{
   if( i >= m_Custom.size() )
      return( 0.0 );

   double v = m_Custom[i];

   if( getCustomQuantizeEnabled() )
   {
      int x = (int)( ( v * getCustomQuantize() ) + ( v < 0.0 ? -0.5 : 0.5 ) );
      v = (double)x / (double)getCustomQuantize();
   }

   return( v );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of values of the custom waveform
*/
/*----------------------------------------------------------------------------*/
size_t LFO::getNumCustomValues() const
{
   return( m_Custom.size() );
}



/*----------------------------------------------------------------------------*/
/*! 2024-06-28
This is called on a regular basis to calculate the next output value of the LFO.
\param s The number of seconds since the last cycle
\param bpm The host's tempo in bpm
*/
/*----------------------------------------------------------------------------*/
void LFO::step( double s, double bpm )
{
   double delaySecs = getDelaySecs();
   if( getDelaySyncEnabled() )
   {
      delaySecs = ( 60.0 * getDelayBeats() ) / bpm;
   }

   double fadeInSecs = getFadeInSecs();
   if( getFadeInSyncEnabled() )
   {
      fadeInSecs = ( 60.0 * getFadeInBeats() ) / bpm;
   }

   double freq = m_Frequency;
   if( m_SyncEnabled )
   {
      freq = bpm / ( 60 * m_SyncBeats );
   }

   m_TimeS += s;

   if( m_TimeS < delaySecs )
   {
      m_Value = 0.0;
      return;
   }

   double amp = 1.0;
   if( m_TimeS - delaySecs < fadeInSecs )
   {
      amp = ( m_TimeS - delaySecs ) / fadeInSecs;
   }

   m_Period += ( s * freq );
   size_t nCycle = (size_t)( m_Period );
   bool stepped = false;
   if( nCycle != m_PeriodCount )
   {
      stepped = true;
      m_PeriodCount = nCycle;
   }

   double period = m_Period + m_StartPhase;
   period -= floor( period );

   if( getOnceEnabled() && ( m_Waveform != Waveform_Random ) && ( m_PeriodCount > 0 ) )
   {
      m_Value = 0.0;
      return;
   }

   if( m_Waveform == Waveform_Sine )
      m_Value = sin( 2.0 * M_PI * period );
   else
   if( m_Waveform == Waveform_Triangle )
   {
      if( period <= 0.25 )
      {
         m_Value = 4.0 * period;
      } else
      if( period <= 0.75 )
      {
         m_Value = 1.0 - ( 4.0 * ( period - 0.25 ) );
      } else
      if( period <= 1.0 )
      {
         m_Value = -1.0 + ( 4.0 * ( period - 0.75 ) );
      }
   } else
   if( m_Waveform == Waveform_Pulse )
      m_Value = period < 0.5 ? 0.0 : 1.0;
   else
   if( m_Waveform == Waveform_Rectangle )
      m_Value = period < 0.5 ? -1.0 : 1.0;
   else
   if( m_Waveform == Waveform_Sawtooth )
      m_Value = 1.0 - ( 2.0 * period );
   else
   if( m_Waveform == Waveform_Random )
   {
      if( stepped )
      {
         m_Value = util::randomValue( -1.0, 1.0 );
      }
   } else
   if( m_Waveform == Waveform_Custom )
   {
      m_Value = getCustomValue( (size_t)( period * getNumCustomValues() ) );
   }

   m_Value *= amp;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Reconstruct an LFO object from a previously generated XML element (see toXml()).
\param pe The XML element
\return Pointer to the LFO object or nullptr on error
*/
/*----------------------------------------------------------------------------*/
LFO *LFO::fromXml( xmlNode *pe )
{
   if( std::string( (char*)pe->name ) != "lfo" )
      return( nullptr );

   LFO *pLFO = new LFO();

   for( xmlNode *pChild = pe->children; pChild; pChild = pChild->next )
   {
      std::string tagName = std::string( (char*)pChild->name );

      if( tagName == "waveform" )
      {
         pLFO->m_Waveform = waveformFromString( util::toLower( util::trim( std::string( (char*)pChild->children->content ) ) ) );
      } else
      if( tagName == "frequency" )
      {
         pLFO->m_Frequency = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "syncenabled" )
      {
         pLFO->m_SyncEnabled = util::toLower( util::trim( std::string( (char*)pChild->children->content ) ) ) == "true";
      } else
      if( tagName == "syncbeats" )
      {
         pLFO->m_SyncBeats = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "delaysyncenabled" )
      {
         pLFO->m_DelaySyncEnabled = util::toLower( util::trim( std::string( (char*)pChild->children->content ) ) ) == "true";
      } else
      if( tagName == "delaysecs" )
      {
         pLFO->m_DelaySecs = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "delaybeats" )
      {
         pLFO->m_DelayBeats = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "fadeinsyncenabled" )
      {
         pLFO->m_FadeInSyncEnabled = util::toLower( util::trim( std::string( (char*)pChild->children->content ) ) ) == "true";
      } else
      if( tagName == "fadeinsecs" )
      {
         pLFO->m_FadeInSecs = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "fadeinbeats" )
      {
         pLFO->m_FadeInBeats = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "onceenabled" )
      {
         pLFO->m_OnceEnabled = util::toLower( util::trim( std::string( (char*)pChild->children->content ) ) ) == "true";
      } else
      if( tagName == "randomphaseenabled" )
      {
         pLFO->m_RandomPhaseEnabled = util::toLower( util::trim( std::string( (char*)pChild->children->content ) ) ) == "true";
      } else
      if( tagName == "custom" )
      {
         for( xmlAttr *pAttr = pChild->properties; pAttr; pAttr = pAttr->next )
         {
            if( pAttr->type == XML_ATTRIBUTE_NODE )
            {
               std::string name = std::string( (char*)pAttr->name );
               xmlChar* pValue = xmlNodeListGetString( pe->doc, pAttr->children, 1 );
               std::string value = std::string( (char*)pValue );
               xmlFree( pValue );

               if( name == "quantizeenabled" )
               {
                  pLFO->m_CustomQuantizeEnabled = util::toLower( util::trim( value ) ) == "true";
               } else
               if( name == "quantize" )
               {
                  pLFO->m_CustomQuantize = std::stoul( util::trim( value ) );
               }
            }
         }

         std::vector<std::string> sVals =
            util::strsplit(
               util::trim(
                  std::string( (char*)pChild->children->content ) ), ",", false );
         std::vector<double> dVals;
         for( size_t j = 0; j < sVals.size(); j++ )
         {
            dVals.push_back( std::stod( sVals[j] ) );
         }
         pLFO->m_Custom = dVals;
      }
   }

   return( pLFO );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Create an XML element from the LFO settings.
\return Pointer to the new XML element
*/
/*----------------------------------------------------------------------------*/
xmlNode *LFO::toXml() const
{
   xmlNode *pe = xmlNewNode( nullptr, (xmlChar *)"lfo" );

   xmlNode *peWaveform = xmlNewNode( nullptr, (xmlChar *)"waveform" );
   xmlAddChild( peWaveform, xmlNewText( (xmlChar *)stdformat( "{}", toString( m_Waveform ) ).c_str() ) );
   xmlAddChild( pe, peWaveform );

   xmlNode *peFrequency = xmlNewNode( nullptr, (xmlChar *)"frequency" );
   xmlAddChild( peFrequency, xmlNewText( (xmlChar *)stdformat( "{}", m_Frequency ).c_str() ) );
   xmlAddChild( pe, peFrequency );

   xmlNode *peSyncEnabled = xmlNewNode( nullptr, (xmlChar *)"syncenabled" );
   xmlAddChild( peSyncEnabled, xmlNewText( (xmlChar *)( m_SyncEnabled ? "true" : "false" ) ) );
   xmlAddChild( pe, peSyncEnabled );

   xmlNode *peSyncBeats = xmlNewNode( nullptr, (xmlChar *)"syncbeats" );
   xmlAddChild( peSyncBeats, xmlNewText( (xmlChar *)stdformat( "{:.2f}", m_SyncBeats ).c_str() ) );
   xmlAddChild( pe, peSyncBeats );

   xmlNode *peDelaySyncEnabled = xmlNewNode( nullptr, (xmlChar *)"delaysyncenabled" );
   xmlAddChild( peDelaySyncEnabled, xmlNewText( (xmlChar *)( m_DelaySyncEnabled ? "true" : "false" ) ) );
   xmlAddChild( pe, peDelaySyncEnabled );

   xmlNode *peDelaySecs = xmlNewNode( nullptr, (xmlChar *)"delaysecs" );
   xmlAddChild( peDelaySecs, xmlNewText( (xmlChar *)stdformat( "{:.2f}", m_DelaySecs ).c_str() ) );
   xmlAddChild( pe, peDelaySecs );

   xmlNode *peDelayBeats = xmlNewNode( nullptr, (xmlChar *)"delaybeats" );
   xmlAddChild( peDelayBeats, xmlNewText( (xmlChar *)stdformat( "{:.2f}", m_DelayBeats ).c_str() ) );
   xmlAddChild( pe, peDelayBeats );

   xmlNode *peFadeInSyncEnabled = xmlNewNode( nullptr, (xmlChar *)"fadeinsyncenabled" );
   xmlAddChild( peFadeInSyncEnabled, xmlNewText( (xmlChar *)( m_FadeInSyncEnabled ? "true" : "false" ) ) );
   xmlAddChild( pe, peFadeInSyncEnabled );

   xmlNode *peFadeInSecs = xmlNewNode( nullptr, (xmlChar *)"fadeinsecs" );
   xmlAddChild( peFadeInSecs, xmlNewText( (xmlChar *)stdformat( "{:.2f}", m_FadeInSecs ).c_str() ) );
   xmlAddChild( pe, peFadeInSecs );

   xmlNode *peFadeInBeats = xmlNewNode( nullptr, (xmlChar *)"fadeinbeats" );
   xmlAddChild( peFadeInBeats, xmlNewText( (xmlChar *)stdformat( "{:.2f}", m_FadeInBeats ).c_str() ) );
   xmlAddChild( pe, peFadeInBeats );

   xmlNode *peOnceEnabled = xmlNewNode( nullptr, (xmlChar *)"onceenabled" );
   xmlAddChild( peOnceEnabled, xmlNewText( (xmlChar *)( m_OnceEnabled ? "true" : "false" ) ) );
   xmlAddChild( pe, peOnceEnabled );

   xmlNode *peRandomPhaseEnabled = xmlNewNode( nullptr, (xmlChar *)"randomphaseenabled" );
   xmlAddChild( peRandomPhaseEnabled, xmlNewText( (xmlChar *)( m_RandomPhaseEnabled ? "true" : "false" ) ) );
   xmlAddChild( pe, peRandomPhaseEnabled );

   xmlNode *peCustom = xmlNewNode( nullptr, (xmlChar *)"custom" );
   xmlNewProp( peCustom, (xmlChar *)"quantizeenabled", (xmlChar *)( m_CustomQuantizeEnabled ? "true" : "false" ) );
   xmlNewProp( peCustom, (xmlChar *)"quantize", (xmlChar *)stdformat( "{}", m_CustomQuantize ).c_str() );
   std::vector<std::string> vals;
   for( size_t i = 0; i < m_Custom.size(); i++ )
   {
      vals.push_back( stdformat( "{:.2f}", m_Custom[i] ) );
   }
   xmlAddChild( peCustom, xmlNewText( (xmlChar *)util::strjoin( vals, "," ).c_str() ) );
   xmlAddChild( pe, peCustom );

   return( pe );
}
