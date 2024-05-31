#ifndef __VOICE_H__
#define __VOICE_H__

#define MODSTEP_SAMPLES 128

#include "Sample.h"

//==============================================================================
namespace SamplerEngine
{
   class Part;

   class Voice
   {
   public:
      Voice( const Part *pPart, const Sample *pSample, int note, int velocity );
      ~Voice();

      bool process( float *pLeft, float *pRight, size_t nSamples, double sampleRate, double bpm );
      const Sample *sample() const;
      int midiNote() const;
      void noteOff();

   protected:
      bool handleLoop();
      void handleModulations( double sampleRate, double bpm );
      double getModValue( ModMatrix::ModSrc modSrc, double defaultValue ) const;

   private:
      double getPanning() const;
      float getLeftAmp( float pan );
      float getRightAmp( float pan );

      const Part *m_pPart;
      const Sample *m_pSample;
      ENV *m_pAEG;
      ENV *m_pEG2;
      std::vector<LFO *> m_LFOs;
      Filter *m_pFilter;
      bool m_NoteIsOn;
      int m_Note;
      double m_PitchMod;
      double m_PanMod;
      double m_AmpMod;
      int m_Velocity;
      double m_Ofs;
      unsigned long m_nSample;
      double m_RandomBipolar;
   };
}

#endif
