#include <math.h>
#include "Voice.h"

Voice::Voice( const Sample *pSample, int note, int velocity ) :
   m_pSample( pSample ),
   m_Note( note ),
   m_Velocity( velocity ),
   m_Ofs( 0.0 ),
   m_NoteIsOn( true ),
   m_nSample( 0 )
{
   if( pSample->getReverse() )
   {
      m_Ofs = pSample->getWave()->numSamples() - 1;
   }

   m_pAEG = new ENV( *pSample->getAEG() );
   m_pAEG->noteOn();
}


Voice::~Voice()
{
   delete m_pAEG;
}


void Voice::noteOff()
{
   if( !m_NoteIsOn )
      return;

   m_NoteIsOn = false;
   m_pAEG->noteOff();
}


int Voice::midiNote() const
{
   return( m_Note );
}


const Sample *Voice::sample() const
{
   return( m_pSample );
}


bool Voice::handleLoop()
{
   const WaveFile *pWav = m_pSample->getWave();
   Sample::PlayMode pm = m_pSample->getPlayMode();

   if( !m_NoteIsOn &&
      ( pm == Sample::PlayModeStandard || pm == Sample::PlayModeLoopUntilRelease ) )
   {
      return( false );
   }

   if( ( pm == Sample::PlayModeStandard ) ||
       ( pm == Sample::PlayModeShot ) )
   {
      if( ( m_Ofs >= m_pSample->getWave()->numSamples() ) ||
          ( m_Ofs < 0 ) )
         return( false );
      else
         return( true );
   }

   if( ( m_pSample->getPlayMode() == Sample::PlayModeLoop ) ||
       ( m_pSample->getPlayMode() == Sample::PlayModeLoopUntilRelease ) )
   {
      if( !m_pSample->getReverse() )
      {
         if( m_Ofs >= pWav->loopEnd() )
         {
            m_Ofs -= pWav->loopEnd() - pWav->loopStart();
         }
      } else
      {
         if( m_Ofs <= pWav->loopStart() )
         {
            m_Ofs += pWav->loopEnd() - pWav->loopStart();
         }
      }
   }

   return( true );
}


void Voice::handleModulations( double sampleRate)
{
   m_nSample++;
   if( m_nSample % MODSTEP_SAMPLES == 0 )
   {
      double secs = (double)MODSTEP_SAMPLES / sampleRate;
      m_pAEG->step( secs );
   }
}


bool Voice::process( float *pLeft, float *pRight, int nSamples, double sampleRate )
{
   float f = (float)m_pSample->getWave()->sampleRate() * pow( 2.0, (double)( (double)m_Note + ( (double)m_pSample->getDetune() / 100.0 ) - m_pSample->getBaseNote() ) / 12.0 );
   if( m_pSample->getReverse() )
   {
      f = -f;
   }

   float relSpeed = f / sampleRate;
   uint8_t *pData = m_pSample->getWave()->data8();
   float velocity = (float)m_Velocity / 127.0;

   if( m_pSample->getWave()->numBits() == 16 )
   {
      if( m_pSample->getWave()->numChannels() == 2 )
      {
         for( int i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = m_Ofs;

            int16_t lv = pData[( 4 * o ) + 0] | ( pData[( 4 * o ) + 1] << 8 );
            int16_t rv = pData[( 4 * o ) + 2] | ( pData[( 4 * o ) + 3] << 8 );

            float lAmp = getLeftAmp( m_pSample->getPan() ) * m_pSample->getGain();
            float rAmp = getRightAmp( m_pSample->getPan() ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= m_pAEG->getValue();
               rAmp *= m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)lv / 32768 ) * lAmp;
            pRight[i] += velocity * ( (float)rv / 32768 ) * rAmp;

            handleModulations( sampleRate );

            m_Ofs += relSpeed;
         }
      } else
      if( m_pSample->getWave()->numChannels() == 1 )
      {
         for( int i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = m_Ofs;

            int16_t v = pData[( 2 * o ) + 0] | ( pData[( 2 * o ) + 1] << 8 );

            float lAmp = getLeftAmp( m_pSample->getPan() ) * m_pSample->getGain();
            float rAmp = getRightAmp( m_pSample->getPan() ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= m_pAEG->getValue();
               rAmp += m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)v / 32768 ) * lAmp;
            pRight[i] += velocity * ( (float)v / 32768 ) * rAmp;

            handleModulations( sampleRate );

            m_Ofs += relSpeed;
         }
      }
   } else
   if( m_pSample->getWave()->numBits() == 8 )
   {
      if( m_pSample->getWave()->numChannels() == 2 )
      {
         for( int i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = m_Ofs;

            int8_t lv = pData[( o * 2 ) + 0];
            int8_t rv = pData[( o * 2 ) + 1];

            float lAmp = getLeftAmp( m_pSample->getPan() ) * m_pSample->getGain();
            float rAmp = getRightAmp( m_pSample->getPan() ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= m_pAEG->getValue();
               rAmp *= m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)lv / 128 ) * lAmp;
            pRight[i] += velocity * ( (float)rv / 128 ) * rAmp;

            handleModulations( sampleRate );

            m_Ofs += relSpeed;
         }
      } else
      if( m_pSample->getWave()->numChannels() == 1 )
      {
         for( int i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = m_Ofs;

            int8_t v = pData[o];

            float lAmp = getLeftAmp( m_pSample->getPan() ) * m_pSample->getGain();
            float rAmp = getRightAmp( m_pSample->getPan() ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= m_pAEG->getValue();
               rAmp *= m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)v / 128 ) * lAmp;
            pRight[i] += velocity * ( (float)v / 128 ) * rAmp;

            handleModulations( sampleRate );

            m_Ofs += relSpeed;
         }
      }
   }

   return( true );
}


float Voice::getLeftAmp( float pan )
{
   if( pan < -1.0 )
      pan = -1.0;
   else
   if( pan > 1.0 )
      pan = 1.0;

   if( pan < 0 )
      return( 1.0 );
   else
      return( 1.0 - pan );
}


float Voice::getRightAmp( float pan )
{
   if( pan < -1.0 )
      pan = -1.0;
   else
      if( pan > 1.0 )
         pan = 1.0;

   if( pan > 0.0 )
      return( 1.0 );
   else
      return( 1.0 + pan );
}
