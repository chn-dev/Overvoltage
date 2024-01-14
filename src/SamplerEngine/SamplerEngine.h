#ifndef __SAMPLERENGINE_H__
#define __SAMPLERENGINE_H__

#include "Part.h"
#include "Voice.h"

namespace SamplerEngine
{
   class OutputBus
   {
   public:
      OutputBus();
      OutputBus( size_t numSamples, std::vector<float *> writePointers );
      ~OutputBus();

      size_t getNumSamples() const;
      std::vector<float *> &getWritePointers();
      bool isValid() const;

   private:
      bool m_Valid;
      size_t m_NumSamples;
      std::vector<float *> m_WritePointers;
   };

   class Engine
   {
   public:
      Engine();
      ~Engine();

      bool process( std::vector<OutputBus> &buses, double sampleRate );

      void deleteSample( size_t part, Sample *pSample );
      std::list<Sample *> samples( size_t nPart ) const;

      void noteOn( size_t nPart, int note, int vel );
      void noteOff( size_t nPart, int note, int vel );

      std::list<Sample *> &samples( size_t nPart );
      const std::list<Sample *> &constSamples( size_t nPart ) const;

      bool isPlaying( const Sample *pSample ) const;

      static Engine *fromXml( const juce::XmlElement *peOvervoltage );
      juce::XmlElement *toXml() const;

   private:
      std::list<Sample *> getSamplesByMidiNoteAndVelocity( size_t part, int note, int vel ) const;
      void stopVoice( const Voice *pVoice );
      std::set<int> allPlayingMidiNotes() const;

   private:
      std::multimap<int, Voice *> m_Voices;
      std::vector<Part *> m_Parts;

   };
}

#endif

