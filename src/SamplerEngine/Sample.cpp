/*----------------------------------------------------------------------------*/
/*!
\file Sample.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a Sample
*/
/*----------------------------------------------------------------------------*/
#include "Sample.h"

#include "util.h"

using namespace SamplerEngine;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
Sample::Sample( std::string name, WaveFile *pWave, int minNote, int maxNote, int nLayer ) :
   m_Name( name ),
   m_OutputBus( 0 ),
   m_pAEG( nullptr ),
   m_pEG2( nullptr ),
   m_pWave( pWave ),
   m_PlayMode( Sample::PlayModeStandard ),
   m_pFilter( nullptr ),
   m_pModMatrix( nullptr ),
   m_DetuneCents( 0.0 ),
   m_Pan( 0.0 ),
   m_Gain( 1.0 ),
   m_Keytrack( 100.0 ),
   m_PitchbendRange( 2.0 ),
   m_Reverse( false ),
   m_BaseNote( minNote ),
   m_MinNote( minNote ),
   m_MaxNote( maxNote ),
   m_MinVelocity( 0 ),
   m_MaxVelocity( 127 ),
   m_NLayer( nLayer )
{
   m_pAEG = new ENV();
   m_pEG2 = new ENV();

   for( int i = 0; i < NUM_LFO; i++ )
   {
      m_LFOs.push_back( new LFO() );
   }
   m_pFilter = new Filter();
   m_pModMatrix = new ModMatrix( SAMPLERENGINE_NUMMODSLOTS );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
Sample::Sample() :
   m_Name( "" ),
   m_OutputBus( 0 ),
   m_pAEG( nullptr ),
   m_pEG2( nullptr ),
   m_pWave( nullptr ),
   m_PlayMode( Sample::PlayModeStandard ),
   m_pFilter( nullptr ),
   m_pModMatrix( nullptr ),
   m_DetuneCents( 0.0 ),
   m_Pan( 0.0 ),
   m_Gain( 1.0 ),
   m_Keytrack( 100.0 ),
   m_PitchbendRange( 2.0 ),
   m_Reverse( false ),
   m_BaseNote( -1 ),
   m_MinNote( -1 ),
   m_MaxNote( -1 ),
   m_MinVelocity( -1 ),
   m_MaxVelocity( -1 ),
   m_NLayer( 0 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
Sample::~Sample()
{
   delete m_pWave;
   delete m_pAEG;
   delete m_pEG2;
   delete m_pFilter;
   delete m_pModMatrix;

   for( LFO *pLFO : m_LFOs )
   {
      delete pLFO;
   }
   m_LFOs.clear();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Create an XML element from the Sample settings.
\return Pointer to the new XML element
*/
/*----------------------------------------------------------------------------*/
xmlNode *Sample::toXml() const
{
   xmlNode *peSample = xmlNewNode( nullptr, (xmlChar *)"sample" );
   xmlNewProp( peSample, (xmlChar *)"name", (xmlChar *)m_Name.c_str() );

   xmlNode *pePlaymode = xmlNewNode( nullptr, (xmlChar *)"playmode" );
   xmlAddChild( pePlaymode, xmlNewText( (xmlChar *)toString( m_PlayMode ).c_str() ) );
   xmlAddChild( peSample, pePlaymode );

   xmlNode *peDetune = xmlNewNode( nullptr, (xmlChar *)"detune" );
   xmlAddChild( peDetune, xmlNewText( (xmlChar *)stdformat( "{}", m_DetuneCents ).c_str() ) );
   xmlAddChild( peSample, peDetune );

   xmlNode *pePan = xmlNewNode( nullptr, (xmlChar *)"pan" );
   xmlAddChild( pePan, xmlNewText( (xmlChar *)stdformat( "{}", m_Pan ).c_str() ) );
   xmlAddChild( peSample, pePan );

   xmlNode *peGain = xmlNewNode( nullptr, (xmlChar *)"gain" );
   xmlAddChild( peGain, xmlNewText( (xmlChar *)stdformat( "{}", m_Gain ).c_str() ) );
   xmlAddChild( peSample, peGain );

   xmlNode *peKeytrack = xmlNewNode( nullptr, (xmlChar *)"keytrack" );
   xmlAddChild( peKeytrack, xmlNewText( (xmlChar *)stdformat( "{}", m_Keytrack ).c_str() ) );
   xmlAddChild( peSample, peKeytrack );

   xmlNode *pePitchbendRange = xmlNewNode( nullptr, (xmlChar *)"pitchbendrange" );
   xmlAddChild( pePitchbendRange, xmlNewText( (xmlChar *)stdformat( "{}", m_PitchbendRange ).c_str() ) );
   xmlAddChild( peSample, pePitchbendRange );

   xmlNode *peReverse = xmlNewNode( nullptr, (xmlChar *)"reverse" );
   xmlAddChild( peReverse, xmlNewText( (xmlChar *)( m_Reverse ? "true" : "false" ) ) );
   xmlAddChild( peSample, peReverse );

   xmlNode *peBaseNote = xmlNewNode( nullptr, (xmlChar *)"basenote" );
   xmlAddChild( peBaseNote, xmlNewText( (xmlChar *)stdformat( "{}", m_BaseNote ).c_str() ) );
   xmlAddChild( peSample, peBaseNote );

   xmlNode *peMinNote = xmlNewNode( nullptr, (xmlChar *)"minnote" );
   xmlAddChild( peMinNote, xmlNewText( (xmlChar *)stdformat( "{}", m_MinNote ).c_str() ) );
   xmlAddChild( peSample, peMinNote );

   xmlNode *peMaxNote = xmlNewNode( nullptr, (xmlChar *)"maxnote" );
   xmlAddChild( peMaxNote, xmlNewText( (xmlChar *)stdformat( "{}", m_MaxNote ).c_str() ) );
   xmlAddChild( peSample, peMaxNote );

   xmlNode *peMinVelocity = xmlNewNode( nullptr, (xmlChar *)"minvelocity" );
   xmlAddChild( peMinVelocity, xmlNewText( (xmlChar *)stdformat( "{}", m_MinVelocity ).c_str() ) );
   xmlAddChild( peSample, peMinVelocity );

   xmlNode *peMaxVelocity = xmlNewNode( nullptr, (xmlChar *)"maxvelocity" );
   xmlAddChild( peMaxVelocity, xmlNewText( (xmlChar *)stdformat( "{}", m_MaxVelocity ).c_str() ) );
   xmlAddChild( peSample, peMaxVelocity );

   xmlNode *peLayer = xmlNewNode( nullptr, (xmlChar *)"layer" );
   xmlAddChild( peLayer, xmlNewText( (xmlChar *)stdformat( "{}", m_NLayer ).c_str() ) );
   xmlAddChild( peSample, peLayer );

   xmlNode *peOutputBus = xmlNewNode( nullptr, (xmlChar *)"outputbus" );
   xmlAddChild( peOutputBus, xmlNewText( (xmlChar *)stdformat( "{}", m_OutputBus ).c_str() ) );
   xmlAddChild( peSample, peOutputBus );

   xmlNode *peAEG = m_pAEG->toXml();
   xmlNewProp( peAEG, (xmlChar *)"type", (xmlChar *)"amplitude" );
   xmlAddChild( peSample, peAEG );

   xmlNode *peEG2 = m_pEG2->toXml();
   xmlNewProp( peEG2, (xmlChar *)"type", (xmlChar *)"eg2" );
   xmlAddChild( peSample, peEG2 );

   for( size_t i = 0; i < m_LFOs.size(); i++ )
   {
      if( m_LFOs[i] )
      {
         xmlNode *peLFO = m_LFOs[i]->toXml();
         xmlNewProp( peLFO, (xmlChar *)"num", (xmlChar *)stdformat( "{}", i ).c_str() );
         xmlAddChild( peSample, peLFO );
      }
   }

   xmlNode *peFilter = m_pFilter->toXml();
   xmlAddChild( peSample, peFilter );

   xmlNode *peModMatrix = m_pModMatrix->toXml();
   xmlAddChild( peSample, peModMatrix );

   xmlNode *peWave = m_pWave->toXml();
   xmlAddChild( peSample, peWave );

   return( peSample );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Reconstruct a Sample object from a previously generated XML element (see toXml()).
\param pe The XML element
\return Pointer to the Sample object or nullptr on error
*/
/*----------------------------------------------------------------------------*/
Sample *Sample::fromXml( xmlNode *pe )
{
   if( std::string( (char*)pe->name ) != "sample" )
      return( nullptr );

   std::string sampleName;

   for( xmlAttr *pAttr = pe->properties; pAttr; pAttr = pAttr->next )
   {
      if( pAttr->type == XML_ATTRIBUTE_NODE )
      {
         std::string name = std::string( (char*)pAttr->name );
         xmlChar* pValue = xmlNodeListGetString( pe->doc, pAttr->children, 1 );
         std::string value = std::string( (char*)pValue );
         xmlFree( pValue );

         if( name == "name" )
         {
            sampleName = std::string( value );
         }
      }
   }

   PlayMode playMode = PlayModeStandard;
   float detune = 0.0;
   float pan = 0.0;
   float gain = 1.0;
   float keytrack = 100.0;
   float pitchbendRange = 2.0;
   bool reverse = false;
   int baseNote = -1;
   int minNote = -1;
   int maxNote = -1;
   int minVelocity = -1;
   int maxVelocity = -1;
   int outputBus = 0;
   int nLayer = 0;
   ENV *pAEG = nullptr;
   ENV *pEG2 = nullptr;
   std::vector<LFO *> lfos;
   Filter *pFilter = nullptr;
   ModMatrix *pModMatrix = nullptr;
   WaveFile *pWave = nullptr;

   for( xmlNode *pChild = pe->children; pChild; pChild = pChild->next )
   {
      std::string tagName = std::string( (char*)pChild->name );

      if( tagName == "playmode" )
      {
         playMode = fromString( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "detune" )
      {
         detune = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "pan" )
      {
         pan = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "gain" )
      {
         gain = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "keytrack" )
      {
         keytrack = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "pitchbendrange" )
      {
         pitchbendRange = std::stof( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "reverse" )
      {
         reverse = std::string( (char*)pChild->children->content ) == "true";
      } else
      if( tagName == "basenote" )
      {
         baseNote = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "minnote" )
      {
         minNote = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "maxnote" )
      {
         maxNote = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "minvelocity" )
      {
         minVelocity = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "maxvelocity" )
      {
         maxVelocity = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "layer" )
      {
         nLayer = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "outputbus" )
      {
         outputBus = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "filter" )
      {
         pFilter = Filter::fromXml( pChild );
      } else
      if( tagName == "modmatrix" )
      {
         pModMatrix = ModMatrix::fromXml( pChild );
      } else
      if( tagName == "lfo" )
      {
         size_t num = 0;
         for( xmlAttr *pAttr = pChild->properties; pAttr; pAttr = pAttr->next )
         {
            if( pAttr->type == XML_ATTRIBUTE_NODE )
            {
               std::string name = std::string( (char*)pAttr->name );
               xmlChar* pValue = xmlNodeListGetString( pe->doc, pAttr->children, 1 );
               std::string value = std::string( (char*)pValue );
               xmlFree( pValue );

               if( name == "num" )
               {
                  num = (size_t)std::stoi( value );
               }
            }
         }

         LFO *pLFO = LFO::fromXml( pChild );
         if( pLFO )
         {
            if( ( num + 1 ) >= lfos.size() )
               lfos.resize( num + 1 );
            lfos[num] = pLFO;
         }
      } else
      if( tagName == "envelope" )
      {
         std::string envType;

         for( xmlAttr *pAttr = pChild->properties; pAttr; pAttr = pAttr->next )
         {
            if( pAttr->type == XML_ATTRIBUTE_NODE )
            {
               std::string name = std::string( (char*)pAttr->name );
               xmlChar* pValue = xmlNodeListGetString( pe->doc, pAttr->children, 1 );
               std::string value = std::string( (char*)pValue );
               xmlFree( pValue );

               if( name == "type" )
               {
                  envType = value;
               }
            }
         }

         ENV *pENV = ENV::fromXml( pChild );
         if( pENV )
         {
            if( envType == "amplitude" )
            {
               pAEG = pENV;
            } else
            if( envType == "eg2" )
            {
            	pEG2 = pENV;
            } else
            {
               delete pENV;
            }
         }
      } else
      if( tagName == "wave" )
      {
         pWave = WaveFile::fromXml( pChild );
         pWave->dft();
      }
   }

   // Filter is optional
   if( !pFilter )
   {
      pFilter = new Filter();
   }

   if( !pEG2 )
   {
      pEG2 = new ENV();
   }

   if( !pModMatrix )
   {
      pModMatrix = new ModMatrix();
   }

   if( lfos.size() < NUM_LFO )
   {
      lfos.resize( NUM_LFO );
   }

   for( size_t i = 0; i < lfos.size(); i++ )
   {
      if( !lfos[i] )
      {
         lfos[i] = new LFO();
      }
   }

   if( pWave && !sampleName.empty() &&
       baseNote >= 0 && minNote >= 0 &&
       maxNote >= 0 && minVelocity >= 0 &&
       outputBus >= 0 &&
       maxVelocity >= 0 &&
       pAEG && pEG2 && pFilter && pModMatrix )
   {
      Sample *pSample = new Sample();

      pSample->m_Name = sampleName;
      pSample->m_pAEG = pAEG;
      pSample->m_pEG2 = pEG2;
      pSample->m_LFOs = lfos;
      pSample->m_pFilter = pFilter;
      pSample->m_pModMatrix = pModMatrix;
      pSample->m_pWave = pWave;
      pSample->m_PlayMode = playMode;
      pSample->m_DetuneCents = detune;
      pSample->m_Pan = pan;
      pSample->m_Gain = gain;
      pSample->m_Keytrack = keytrack;
      pSample->m_PitchbendRange = pitchbendRange;
      pSample->m_Reverse = reverse;
      pSample->m_BaseNote = baseNote;
      pSample->m_MinNote = minNote;
      pSample->m_MaxNote = maxNote;
      pSample->m_MinVelocity = minVelocity;
      pSample->m_MaxVelocity = maxVelocity;
      pSample->m_NLayer = nLayer;
      pSample->m_OutputBus = outputBus;

      return( pSample );
   } else
   {
      if( pAEG )
      {
         delete pAEG;
      }

      if( pEG2 )
      {
         delete pEG2;
      }

      if( pFilter )
      {
         delete pFilter;
      }

      if( pModMatrix )
      {
         delete pModMatrix;
      }

      if( pWave )
      {
         delete pWave;
      }

      for( LFO *pLFO : lfos )
      {
         delete pLFO;
      }

      return( nullptr );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The amplitude envelope generator
*/
/*----------------------------------------------------------------------------*/
ENV *Sample::getAEG() const
{
   return( m_pAEG );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Envelope generator #2
*/
/*----------------------------------------------------------------------------*/
ENV *Sample::getEG2() const
{
   return( m_pEG2 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param n The index of the LFO
\return Pointer to LFO
*/
/*----------------------------------------------------------------------------*/
LFO *Sample::getLFO( size_t n ) const
{
   if( n >= m_LFOs.size() )
      return( nullptr );

   return( m_LFOs[n] );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of available LFOs (should be NUM_LFO)
*/
/*----------------------------------------------------------------------------*/
size_t Sample::getNumLFOs() const
{
   return( m_LFOs.size() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Pointer to the filter
*/
/*----------------------------------------------------------------------------*/
Filter *Sample::getFilter() const
{
   return( m_pFilter );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Pointer to the modulation matrix
*/
/*----------------------------------------------------------------------------*/
ModMatrix *Sample::getModMatrix() const
{
   return( m_pModMatrix );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param name The sample's new name
*/
/*----------------------------------------------------------------------------*/
void Sample::setName( std::string name )
{
   m_Name = name;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the sample is to be played in reverse
*/
/*----------------------------------------------------------------------------*/
bool Sample::getReverse() const
{
   return( m_Reverse );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param r true if the sample shall be played in reverse
*/
/*----------------------------------------------------------------------------*/
void Sample::setReverse( bool r )
{
   m_Reverse = r;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pm The playmode
*/
/*----------------------------------------------------------------------------*/
void Sample::setPlayMode( Sample::PlayMode pm )
{
   m_PlayMode = pm;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The playmode
*/
/*----------------------------------------------------------------------------*/
Sample::PlayMode Sample::getPlayMode() const
{
   return( m_PlayMode );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param mode The playmode type
\return A string representation of the playmode type
*/
/*----------------------------------------------------------------------------*/
std::string Sample::toString( Sample::PlayMode mode )
{
   switch( mode )
   {
      case PlayModeStandard:
         return( "Standard" );
         break;
      case PlayModeLoop:
         return( "Loop" );
         break;
      case PlayModeShot:
         return( "Shot" );
         break;
      case PlayModeLoopUntilRelease:
         return( "LoopUntilRelease" );
         break;
      default:
         return( "Standard" );
         break;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param mode The playmode type
\return The string representation of the specified playmode type
*/
/*----------------------------------------------------------------------------*/
Sample::PlayMode Sample::fromString( std::string mode )
{
   if( mode == "Standard" )
   {
      return( PlayModeStandard );
   } else
   if( mode == "Loop" )
   {
      return( PlayModeLoop );
   } else
   if( mode == "Shot" )
   {
      return( PlayModeShot );
   } else
   if( mode == "LoopUntilRelease" )
   {
      return( PlayModeLoopUntilRelease );
   } else
   {
      return( PlayModeStandard );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param note A MIDI note number
\return true if the sample matches the given note number
*/
/*----------------------------------------------------------------------------*/
bool Sample::matchesMidiNote( int note ) const
{
   return( ( m_MinNote <= note ) && ( note <= m_MaxNote ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param vel A MIDI velocity
\return true if the sample matches the given velocity
*/
/*----------------------------------------------------------------------------*/
bool Sample::matchesVelocity( int vel ) const
{
   return( ( m_MinVelocity <= vel ) && ( vel <= m_MaxVelocity ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A set of all possible play modes
*/
/*----------------------------------------------------------------------------*/
std::set<Sample::PlayMode> Sample::allPlayModes()
{
   static std::set<Sample::PlayMode> m;

   if( m.size() == 0 )
   {
      m.insert( Sample::PlayModeStandard );
      m.insert( Sample::PlayModeLoop );
      m.insert( Sample::PlayModeShot );
      m.insert( Sample::PlayModeLoopUntilRelease );
   }

   return( m );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The sample name
*/
/*----------------------------------------------------------------------------*/
std::string Sample::getName() const
{
   return( m_Name );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The wave data
*/
/*----------------------------------------------------------------------------*/
WaveFile *Sample::getWave() const
{
   return( m_pWave );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The base MIDI note number (0..255)
*/
/*----------------------------------------------------------------------------*/
int Sample::getBaseNote() const
{
   return( m_BaseNote );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param note The new base MIDI note number (0..255)
*/
/*----------------------------------------------------------------------------*/
void Sample::setBaseNote( int note )
{
   m_BaseNote = note;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The minimum MIDI note number (0..255)
*/
/*----------------------------------------------------------------------------*/
int Sample::getMinNote() const
{
   return( m_MinNote );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The maximum MIDI note number (0..255)
*/
/*----------------------------------------------------------------------------*/
int Sample::getMaxNote() const
{
   return( m_MaxNote );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param note The new minimum MIDI note number (0..255)
*/
/*----------------------------------------------------------------------------*/
void Sample::setMinNote( int note )
{
   m_MinNote = note;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param note The new maximum MIDI note number (0..255)
*/
/*----------------------------------------------------------------------------*/
void Sample::setMaxNote( int note )
{
   m_MaxNote = note;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The minimum velocity (0..127)
*/
/*----------------------------------------------------------------------------*/
int Sample::getMinVelocity() const
{
   return( m_MinVelocity );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param v The new minimum velocity (0..127)
*/
/*----------------------------------------------------------------------------*/
void Sample::setMinVelocity( int v )
{
   m_MinVelocity = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The maximum velocity (0..127)
*/
/*----------------------------------------------------------------------------*/
int Sample::getMaxVelocity() const
{
   return( m_MaxVelocity );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param v The new maximum velocity (0..127)
*/
/*----------------------------------------------------------------------------*/
void Sample::setMaxVelocity( int v )
{
   m_MaxVelocity = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Make sure that m_MinNote <= m_MaxNote
*/
/*----------------------------------------------------------------------------*/
void Sample::correctMinMaxNote()
{
   if( m_MinNote > m_MaxNote )
   {
      int tmp = m_MinNote;
      m_MinNote = m_MaxNote;
      m_MaxNote = tmp;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The detune value in cents (1/100th of a half-tone)
*/
/*----------------------------------------------------------------------------*/
float Sample::getDetune() const
{
   return( m_DetuneCents );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param d The new detune value in cents (1/100th of a half-tone)
*/
/*----------------------------------------------------------------------------*/
void Sample::setDetune( float d )
{
   m_DetuneCents = d;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The panning value (-1..1)
*/
/*----------------------------------------------------------------------------*/
float Sample::getPan() const
{
   return( m_Pan );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pan The new panning value (-1..1)
*/
/*----------------------------------------------------------------------------*/
void Sample::setPan( float pan )
{
   m_Pan = pan;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The gain value
*/
/*----------------------------------------------------------------------------*/
float Sample::getGain() const
{
   return( m_Gain );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param gain The new gain value
*/
/*----------------------------------------------------------------------------*/
void Sample::setGain( float gain )
{
   m_Gain = gain;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The keytrack value in percent
*/
/*----------------------------------------------------------------------------*/
float Sample::getKeytrack() const
{
   return( m_Keytrack );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param kt The new keytrack value in percent
*/
/*----------------------------------------------------------------------------*/
void Sample::setKeytrack( float kt )
{
   m_Keytrack = kt;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The pitchbend range in halftones
*/
/*----------------------------------------------------------------------------*/
float Sample::getPitchbendRange() const
{
   return( m_PitchbendRange );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pb The new pitchbend range in halftones
*/
/*----------------------------------------------------------------------------*/
void Sample::setPitchbendRange( float pb )
{
   m_PitchbendRange = pb;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The output bus
*/
/*----------------------------------------------------------------------------*/
int Sample::getOutputBus() const
{
   return( m_OutputBus );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param n The new output bus
*/
/*----------------------------------------------------------------------------*/
void Sample::setOutputBus( int n )
{
   m_OutputBus = n;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param nLayer The layer of this sample (0..SAMPLERENGINE_NUMLAYERS-1)
*/
/*----------------------------------------------------------------------------*/
void Sample::setLayer( int nLayer )
{
   m_NLayer = nLayer;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The layer of this sample (0..SAMPLERENGINE_NUMLAYERS-1)
*/
/*----------------------------------------------------------------------------*/
int Sample::getLayer() const
{
   return( m_NLayer );
}

