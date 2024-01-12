#pragma once

#define MODSTEP_SAMPLES 128

#include "Sample.h"

//==============================================================================
class Voice
{
public:
   Voice( const Sample *pSample, int note, int velocity );
   ~Voice();

   bool process( float *pLeft, float *pRight, int nSamples, double sampleRate );
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
   bool m_NoteIsOn;
   int m_Note;
   int m_Velocity;
   double m_Ofs;
   unsigned long m_nSample;
};
