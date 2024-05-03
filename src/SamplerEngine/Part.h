#ifndef __PART_H__
#define __PART_H__

#include <list>
#include "Sample.h"
#include "Voice.h"

//==============================================================================
namespace SamplerEngine
{
   class OutputBus;

   class Part
   {
   public:
      Part( size_t partNum );
      ~Part();

      void noteOn( int note, int vel );
      void noteOff( int note, int vel );
      void setPitchbend( double v );
      double getPitchbend() const;

      std::list<Sample *> &samples();
      const std::list<Sample *> &constSamples() const;

      size_t getPartNum() const;
      void deleteSample( Sample *pSample );

      bool isPlaying( const Sample *pSample ) const;

      static Part *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *toXml() const;

      bool process( std::vector<OutputBus> &buses, double sampleRate, double bpm );

   private:
      std::list<Sample *> getSamplesByMidiNoteAndVelocity( int note, int vel ) const;
      void stopVoice( const Voice *pVoice );

   private:
      size_t m_PartNum;
      double m_Pitchbend;
      std::list<Sample *> m_Samples;
      std::multimap<int, Voice *> m_Voices;
   };
}

#endif
