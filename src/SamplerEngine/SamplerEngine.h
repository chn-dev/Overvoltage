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

      bool process( std::vector<OutputBus> &buses, double sampleRate, double bpm );

      void deleteSample( size_t part, Sample *pSample );
      std::list<Sample *> samples( size_t nPart ) const;

      void noteOn( size_t nPart, int note, int vel );
      void noteOff( size_t nPart, int note, int vel );
      void pitchbend( size_t nPart, double v );

      std::list<Sample *> &samples( size_t nPart );
      const std::list<Sample *> &constSamples( size_t nPart ) const;

      bool isPlaying( size_t nPart, const Sample *pSample ) const;

      static Engine *fromXml( const juce::XmlElement *peOvervoltage );
      juce::XmlElement *toXml() const;

   private:
      std::vector<Part *> m_Parts;
   };
}

#endif

