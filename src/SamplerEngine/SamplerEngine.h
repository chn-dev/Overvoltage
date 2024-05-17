#ifndef __SAMPLERENGINE_H__
#define __SAMPLERENGINE_H__

#include "Part.h"
#include "Voice.h"

#define SAMPLERENGINE_NUMLAYERS 8

class PluginProcessor;

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
      Engine( PluginProcessor *pProcessor = nullptr );
      ~Engine();

      bool process( std::vector<OutputBus> &buses, double sampleRate, double bpm );

      void setProcessor( PluginProcessor *pProcessor );

      void deleteSample( size_t part, Sample *pSample );
      std::list<Sample *> samples( size_t nPart ) const;

      void noteOn( size_t nPart, int note, int vel );
      void noteOff( size_t nPart, int note, int vel );
      void pitchbend( size_t nPart, double v );
      void controllerChange( size_t nPart, int ccNum, double v );

      std::list<Sample *> &samples( size_t nPart );
      const std::list<Sample *> &constSamples( size_t nPart ) const;

      bool isSoloEnabled() const;
      std::set<Sample *> getSelectedSamples() const;

      bool isPlaying( size_t nPart, const Sample *pSample ) const;

      static Engine *fromXml( const juce::XmlElement *peOvervoltage );
      juce::XmlElement *toXml() const;

   private:
      PluginProcessor *m_pProcessor;
      std::vector<Part *> m_Parts;
   };
}

#endif

