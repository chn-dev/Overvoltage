#include "Sample.h"

#include "util.h"

using namespace SamplerEngine;

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
   m_pModMatrix = new ModMatrix();
}


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


juce::XmlElement *Sample::toXml() const
{
   juce::XmlElement *peSample = new juce::XmlElement( "sample" );
   peSample->setAttribute( "name", juce::String( m_Name ) );

   juce::XmlElement *pePlaymode = new juce::XmlElement( "playmode" );
   pePlaymode->addTextElement( toString( m_PlayMode ) );
   peSample->addChildElement( pePlaymode );

   juce::XmlElement *peDetune = new juce::XmlElement( "detune" );
   peDetune->addTextElement( stdformat( "{}", m_DetuneCents ) );
   peSample->addChildElement( peDetune );

   juce::XmlElement *pePan = new juce::XmlElement( "pan" );
   pePan->addTextElement( stdformat( "{}", m_Pan ) );
   peSample->addChildElement( pePan );

   juce::XmlElement *peGain = new juce::XmlElement( "gain" );
   peGain->addTextElement( stdformat( "{}", m_Gain ) );
   peSample->addChildElement( peGain );

   juce::XmlElement *peKeytrack = new juce::XmlElement( "keytrack" );
   peKeytrack->addTextElement( stdformat( "{}", m_Keytrack ) );
   peSample->addChildElement( peKeytrack );

   juce::XmlElement *pePitchbendRange = new juce::XmlElement( "pitchbendrange" );
   pePitchbendRange->addTextElement( stdformat( "{}", m_PitchbendRange ) );
   peSample->addChildElement( pePitchbendRange );

   juce::XmlElement *peReverse = new juce::XmlElement( "reverse" );
   peReverse->addTextElement( m_Reverse ? "true" : "false" );
   peSample->addChildElement( peReverse );

   juce::XmlElement *peBaseNote = new juce::XmlElement( "basenote" );
   peBaseNote->addTextElement( stdformat( "{}", m_BaseNote ) );
   peSample->addChildElement( peBaseNote );

   juce::XmlElement *peMinNote = new juce::XmlElement( "minnote" );
   peMinNote->addTextElement( stdformat( "{}", m_MinNote ) );
   peSample->addChildElement( peMinNote );

   juce::XmlElement *peMaxNote = new juce::XmlElement( "maxnote" );
   peMaxNote->addTextElement( stdformat( "{}", m_MaxNote ) );
   peSample->addChildElement( peMaxNote );

   juce::XmlElement *peMinVelocity = new juce::XmlElement( "minvelocity" );
   peMinVelocity->addTextElement( stdformat( "{}", m_MinVelocity ) );
   peSample->addChildElement( peMinVelocity );

   juce::XmlElement *peMaxVelocity = new juce::XmlElement( "maxvelocity" );
   peMaxVelocity->addTextElement( stdformat( "{}", m_MaxVelocity ) );
   peSample->addChildElement( peMaxVelocity );

   juce::XmlElement *peLayer = new juce::XmlElement( "layer" );
   peLayer->addTextElement( stdformat( "{}", m_NLayer ) );
   peSample->addChildElement( peLayer );

   juce::XmlElement *peOutputBus = new juce::XmlElement( "outputbus" );
   peOutputBus->addTextElement( stdformat( "{}", m_OutputBus ) );
   peSample->addChildElement( peOutputBus );

   juce::XmlElement *peAEG = m_pAEG->toXml();
   peAEG->setAttribute( "type", "amplitude" );
   peSample->addChildElement( peAEG );

   juce::XmlElement *peEG2 = m_pEG2->toXml();
   peEG2->setAttribute( "type", "eg2" );
   peSample->addChildElement( peEG2 );

   for( size_t i = 0; i < m_LFOs.size(); i++ )
   {
      if( m_LFOs[i] )
      {
         juce::XmlElement *peLFO = m_LFOs[i]->toXml();
         peLFO->setAttribute( "num", stdformat( "{}", i ) );
         peSample->addChildElement( peLFO );
      }
   }

   juce::XmlElement *peFilter = m_pFilter->toXml();
   peSample->addChildElement( peFilter );

   juce::XmlElement *peModMatrix = m_pModMatrix->toXml();
   peSample->addChildElement( peModMatrix );

   juce::XmlElement *peWave = m_pWave->toXml();
   peSample->addChildElement( peWave );

   return( peSample );
}


Sample *Sample::fromXml( const juce::XmlElement *pe )
{
   if( !pe )
      return( nullptr );
   if( pe->getTagName() != "sample" )
      return( nullptr );

   std::string name = pe->getStringAttribute( "name" ).toStdString();
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

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "playmode" )
      {
         playMode = fromString( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "detune" )
      {
         detune = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "pan" )
      {
         pan = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "gain" )
      {
         gain = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "keytrack" )
      {
         keytrack = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "pitchbendrange" )
      {
         pitchbendRange = std::stof( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "reverse" )
      {
         reverse = pChild->getChildElement( 0 )->getText() == "true";
      } else
      if( tagName == "basenote" )
      {
         baseNote = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "minnote" )
      {
         minNote = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "maxnote" )
      {
         maxNote = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "minvelocity" )
      {
         minVelocity = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "maxvelocity" )
      {
         maxVelocity = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "layer" )
      {
         nLayer = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "outputbus" )
      {
         outputBus = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
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
         size_t num = (size_t)std::stoi( pChild->getStringAttribute( "num" ).toStdString() );
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
         std::string envType = pChild->getStringAttribute( "type" ).toStdString();

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

   if( pWave && !name.empty() &&
       baseNote >= 0 && minNote >= 0 &&
       maxNote >= 0 && minVelocity >= 0 &&
       outputBus >= 0 &&
       maxVelocity >= 0 &&
       pAEG && pEG2 && pFilter && pModMatrix )
   {
      Sample *pSample = new Sample();

      pSample->m_Name = name;
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


ENV *Sample::getAEG() const
{
   return( m_pAEG );
}


ENV *Sample::getEG2() const
{
   return( m_pEG2 );
}


LFO *Sample::getLFO( size_t n ) const
{
   if( n >= m_LFOs.size() )
      return( nullptr );

   return( m_LFOs[n] );
}


size_t Sample::getNumLFOs() const
{
   return( m_LFOs.size() );
}


Filter *Sample::getFilter() const
{
   return( m_pFilter );
}


ModMatrix *Sample::getModMatrix() const
{
   return( m_pModMatrix );
}


void Sample::setName( std::string name )
{
   m_Name = name;
}


bool Sample::getReverse() const
{
   return( m_Reverse );
}


void Sample::setReverse( bool r )
{
   m_Reverse = r;
}


void Sample::setPlayMode( Sample::PlayMode pm )
{
   m_PlayMode = pm;
}


Sample::PlayMode Sample::getPlayMode() const
{
   return( m_PlayMode );
}


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


bool Sample::matchesMidiNote( int note ) const
{
   return( ( m_MinNote <= note ) && ( note <= m_MaxNote ) );
}


bool Sample::matchesVelocity( int vel ) const
{
   return( ( m_MinVelocity <= vel ) && ( vel <= m_MaxVelocity ) );
}


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


std::string Sample::getName() const
{
   return( m_Name );
}


WaveFile *Sample::getWave() const
{
   return( m_pWave );
}


int Sample::getBaseNote() const
{
   return( m_BaseNote );
}


void Sample::setBaseNote( int note )
{
   m_BaseNote = note;
}


int Sample::getMinNote() const
{
   return( m_MinNote );
}


int Sample::getMaxNote() const
{
   return( m_MaxNote );
}


void Sample::setMinNote( int note )
{
   m_MinNote = note;
}


void Sample::setMaxNote( int note )
{
   m_MaxNote = note;
}


int Sample::getMinVelocity() const
{
   return( m_MinVelocity );
}


void Sample::setMinVelocity( int v )
{
   m_MinVelocity = v;
}


int Sample::getMaxVelocity() const
{
   return( m_MaxVelocity );
}


void Sample::setMaxVelocity( int v )
{
   m_MaxVelocity = v;
}


void Sample::correctMinMaxNote()
{
   if( m_MinNote > m_MaxNote )
   {
      int tmp = m_MinNote;
      m_MinNote = m_MaxNote;
      m_MaxNote = tmp;
   }
}


float Sample::getDetune() const
{
   return( m_DetuneCents );
}


void Sample::setDetune( float d )
{
   m_DetuneCents = d;
}


float Sample::getPan() const
{
   return( m_Pan );
}


void Sample::setPan( float pan )
{
   m_Pan = pan;
}

float Sample::getGain() const
{
   return( m_Gain );
}


void Sample::setGain( float gain )
{
   m_Gain = gain;
}


float Sample::getKeytrack() const
{
   return( m_Keytrack );
}


void Sample::setKeytrack( float kt )
{
   m_Keytrack = kt;
}


float Sample::getPitchbendRange() const
{
   return( m_PitchbendRange );
}


void Sample::setPitchbendRange( float pb )
{
   m_PitchbendRange = pb;
}


int Sample::getOutputBus() const
{
   return( m_OutputBus );
}


void Sample::setOutputBus( int n )
{
   m_OutputBus = n;
}


void Sample::setLayer( int nLayer )
{
   m_NLayer = nLayer;
}


int Sample::getLayer() const
{
   return( m_NLayer );
}
