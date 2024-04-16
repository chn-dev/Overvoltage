#include <math.h>
#include <util.h>
#include "Voice.h"

using namespace SamplerEngine;

Voice::Voice( const Sample *pSample, int note, int velocity ) :
   m_pSample( pSample ),
   m_pAEG( nullptr ),
   m_pEG2( nullptr ),
   m_pFilter( nullptr ),
   m_NoteIsOn( true ),
   m_Note( note ),
   m_PitchMod( 0.0 ),
   m_PanMod( 0.0 ),
   m_Velocity( velocity ),
   m_Ofs( 0.0 ),
   m_nSample( 0 )
{
   if( pSample->getReverse() )
   {
      m_Ofs = (double)pSample->getWave()->numSamples() - 1;
   }

   m_pAEG = new ENV( *pSample->getAEG() );
   m_pAEG->noteOn();

   m_pEG2 = new ENV( *pSample->getEG2() );
   m_pEG2->noteOn();

   m_pFilter = new Filter( *pSample->getFilter() );
}


Voice::~Voice()
{
   delete m_pAEG;
   delete m_pEG2;
   delete m_pFilter;
}


void Voice::noteOff()
{
   if( !m_NoteIsOn )
      return;

   m_NoteIsOn = false;
   m_pAEG->noteOff();
   m_pEG2->noteOff();
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

   if( !m_NoteIsOn && ( pm == Sample::PlayModeLoopUntilRelease ) )
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
   if( m_nSample % MODSTEP_SAMPLES == 0 )
   {
      std::map<ModMatrix::ModDest, double> modValues;

      for( size_t nSlot = 0; nSlot < m_pSample->getModMatrix()->numSlots(); nSlot++ )
      {
         ModMatrix::ModSlot *pSlot = m_pSample->getModMatrix()->getSlot( nSlot );
         ModMatrix::ModSrc modSrc = pSlot->getSrc();
         ModMatrix::ModDest modDest = pSlot->getDest();
         if( modSrc != ModMatrix::ModSrc_None && modDest != ModMatrix::ModDest_None )
         {
            ModMatrix::ModDestInfo modDestInfo = modDest;
            double modVal = 0.0;
            double modAmount = pSlot->getAmount();

            if( modSrc == ModMatrix::ModSrc_AEG )
               modVal = m_pAEG->getValue();
            else
            if( modSrc == ModMatrix::ModSrc_EG2 )
               modVal = m_pEG2->getValue();
            else
            if( modSrc == ModMatrix::ModSrc_Velocity )
              modVal = (double)m_Velocity / 127.0f;
            else
            if( modSrc == ModMatrix::ModSrc_AbsNote )
               modVal = (double)m_Note / 127.0f;
            else
            if( modSrc == ModMatrix::ModSrc_RelNote )
               modVal = 2.0 * (double)( m_Note - m_pSample->getMinNote() ) / (double)( m_pSample->getMaxNote() - m_pSample->getMinNote() );

            modVal = modVal * modAmount;

            if( modValues.find( modDest ) != modValues.end() )
               modValues[modDest] += modVal;
            else
               modValues[modDest] = modVal;
         }
      }

      for( auto mod : modValues )
      {
         ModMatrix::ModDest modDest = mod.first;
         double modVal = mod.second;

         if( modDest == ModMatrix::ModDest_FilterCutoff )
            m_pFilter->setCutoffMod( modVal );
         else
         if( modDest == ModMatrix::ModDest_FilterResonance )
            m_pFilter->setResonanceMod( modVal );
         else
         if( modDest == ModMatrix::ModDest_Pitch )
            m_PitchMod = modVal;
         else
         if( modDest == ModMatrix::ModDest_Pan )
            m_PanMod = modVal;
      }

      double secs = (double)MODSTEP_SAMPLES / sampleRate;
      m_pAEG->step( secs );
      m_pEG2->step( secs );
   }
   m_nSample++;
}


double Voice::getPanning() const
{
   return( util::clamp( -1.0, 1.0, m_pSample->getPan() + ( ( 2.0 * m_PanMod ) / 100.0 ) ) );
}


bool Voice::process( float *pL, float *pR, size_t nSamples, double sampleRate )
{
   std::vector<float> left;
   std::vector<float> right;
   left.resize( nSamples );
   right.resize( nSamples );

   float *pLeft = left.data();;
   float *pRight = right.data();;

   double noteOfs = m_PitchMod * 12.0;
   double f = (double)m_pSample->getWave()->sampleRate() *
      pow( 2.0,
         (double)( (double)m_Note + noteOfs + ( (double)m_pSample->getDetune() / 100.0 ) - (double)m_pSample->getBaseNote() ) / 12.0 );
   if( m_pSample->getReverse() )
   {
      f = -f;
   }

   double relSpeed = f / sampleRate;
   uint8_t *pData = m_pSample->getWave()->data8();
   float velocity = (float)m_Velocity / 127.0f;

   m_pFilter->setCutoff( m_pSample->getFilter()->getCutoff() );
   m_pFilter->setResonance( m_pSample->getFilter()->getResonance() );
   m_pFilter->setType( m_pSample->getFilter()->getType() );

   if( m_pSample->getWave()->numBits() == 16 )
   {
      if( m_pSample->getWave()->numChannels() == 2 )
      {
         for( size_t i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            handleModulations( sampleRate );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = (int)m_Ofs;

            int16_t lv = pData[( 4 * o ) + 0] | ( pData[( 4 * o ) + 1] << 8 );
            int16_t rv = pData[( 4 * o ) + 2] | ( pData[( 4 * o ) + 3] << 8 );

            double panning = getPanning();
            float lAmp = getLeftAmp( panning ) * m_pSample->getGain();
            float rAmp = getRightAmp( panning ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= (float)m_pAEG->getValue();
               rAmp *= (float)m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)lv / 32768 ) * lAmp;
            pRight[i] += velocity * ( (float)rv / 32768 ) * rAmp;

            m_pFilter->process( &pLeft[i], &pRight[i], 1, sampleRate );

            m_Ofs += relSpeed;
         }
      } else
      if( m_pSample->getWave()->numChannels() == 1 )
      {
         for( size_t i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            handleModulations( sampleRate );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = (int)m_Ofs;

            int16_t v = pData[( 2 * o ) + 0] | ( pData[( 2 * o ) + 1] << 8 );

            double panning = getPanning();
            float lAmp = getLeftAmp( panning ) * m_pSample->getGain();
            float rAmp = getRightAmp( panning ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= (float)m_pAEG->getValue();
               rAmp *= (float)m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)v / 32768 ) * lAmp;
            pRight[i] += velocity * ( (float)v / 32768 ) * rAmp;

            m_pFilter->process( &pLeft[i], &pRight[i], 1, sampleRate );

            m_Ofs += relSpeed;
         }
      }
   } else
   if( m_pSample->getWave()->numBits() == 8 )
   {
      if( m_pSample->getWave()->numChannels() == 2 )
      {
         for( size_t i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            handleModulations( sampleRate );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = (int)m_Ofs;

            int8_t lv = (int8_t)pData[( o * 2 ) + 0];
            int8_t rv = (int8_t)pData[( o * 2 ) + 1];

            double panning = getPanning();
            float lAmp = getLeftAmp( panning ) * m_pSample->getGain();
            float rAmp = getRightAmp( panning ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= (float)m_pAEG->getValue();
               rAmp *= (float)m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)lv / 128 ) * lAmp;
            pRight[i] += velocity * ( (float)rv / 128 ) * rAmp;

            m_pFilter->process( &pLeft[i], &pRight[i], 1, sampleRate );

            m_Ofs += relSpeed;
         }
      } else
      if( m_pSample->getWave()->numChannels() == 1 )
      {
         for( size_t i = 0; i < nSamples; i++ )
         {
            if( !handleLoop() )
               return( false );

            handleModulations( sampleRate );

            if( m_pAEG->hasEnded() && m_pSample->getPlayMode() != Sample::PlayModeShot )
               return( false );

            int o = (int)m_Ofs;

            int8_t v = (int8_t)pData[o];

            double panning = getPanning();
            float lAmp = getLeftAmp( panning ) * m_pSample->getGain();
            float rAmp = getRightAmp( panning ) * m_pSample->getGain();
            if( m_pSample->getPlayMode() != Sample::PlayModeShot )
            {
               lAmp *= (float)m_pAEG->getValue();
               rAmp *= (float)m_pAEG->getValue();
            }

            pLeft[i] += velocity * ( (float)v / 128 ) * lAmp;
            pRight[i] += velocity * ( (float)v / 128 ) * rAmp;

            m_pFilter->process( &pLeft[i], &pRight[i], 1, sampleRate );

            m_Ofs += relSpeed;
         }
      }
   }

   for( size_t n = 0; n < nSamples; n++ )
   {
      pL[n] += pLeft[n];
      pR[n] += pRight[n];
   }

   return( true );
}


float Voice::getLeftAmp( float pan )
{
   if( pan < -1.0f )
      pan = -1.0f;
   else
   if( pan > 1.0f )
      pan = 1.0f;

   if( pan < 0.0f )
      return( 1.0f );
   else
      return( 1.0f - pan );
}


float Voice::getRightAmp( float pan )
{
   if( pan < -1.0f )
      pan = -1.0f;
   else
      if( pan > 1.0f )
         pan = 1.0f;

   if( pan > 0.0f )
      return( 1.0f );
   else
      return( 1.0f + pan );
}
