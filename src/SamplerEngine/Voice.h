#ifndef __VOICE_H__
#define __VOICE_H__

#define MODSTEP_SAMPLES 128

#include "Sample.h"

//==============================================================================
namespace SamplerEngine
{
   class Voice
   {
   public:
      Voice( const Sample *pSample, int note, int velocity );
      ~Voice();

      bool process( float *pLeft, float *pRight, size_t nSamples, double sampleRate );
      const Sample *sample() const;
      int midiNote() const;
      void noteOff();

   protected:
      bool handleLoop();
      void handleModulations( double sampleRate );

   private:
      static float getLeftAmp( float pan );
      static float getRightAmp( float pan );

      const Sample *m_pSample;
      ENV *m_pAEG;
      ENV *m_pEG2;
      Filter *m_pFilter;
      bool m_NoteIsOn;
      int m_Note;
      double m_PitchMod;
      double m_PanMod;
      int m_Velocity;
      double m_Ofs;
      unsigned long m_nSample;
   };
}

#endif
