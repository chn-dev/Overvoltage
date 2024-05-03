#ifndef __LFO_H__
#define __LFO_H__

#include "JuceHeader.h"

//==============================================================================
namespace SamplerEngine
{
   class LFO
   {
   public:
      enum Waveform
      {
         Waveform_Sine = 1,
         Waveform_Triangle,
         Waveform_Pulse,
         Waveform_Rectangle,
         Waveform_Sawtooth,
         Waveform_Custom,
         Waveform_None
      };

      LFO();
      LFO( const LFO &d );
      ~LFO();

      void getSettings( const LFO &d );

      double getValue() const;

      Waveform getWaveform() const;
      void setWaveform( Waveform wf );

      double getFrequency() const;
      void setFrequency( double f );

      void noteOn();
      void noteOff();

      static std::string toString( Waveform wf );
      static Waveform waveformFromString( const std::string &wf );
      static std::set<Waveform> allWaveforms();

      void step( double s, double bpm );

      static LFO *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *toXml() const;

   protected:

   private:
      double m_Value;
      double m_Period;
      Waveform m_Waveform;
      double m_Frequency;
   };
}
#endif

