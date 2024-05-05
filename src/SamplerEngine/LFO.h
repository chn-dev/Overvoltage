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
         Waveform_Random,
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

      void setSyncEnabled( bool e );
      bool getSyncEnabled() const;
      void setSyncBeats( double beats );
      double getSyncBeats() const;

      void setDelaySyncEnabled( bool e );
      bool getDelaySyncEnabled() const;
      void setDelayBeats( double beats );
      double getDelayBeats() const;
      void setDelaySecs( double s );
      double getDelaySecs() const;

      void setFadeInSyncEnabled( bool e );
      bool getFadeInSyncEnabled() const;
      void setFadeInBeats( double beats );
      double getFadeInBeats() const;
      void setFadeInSecs( double s );
      double getFadeInSecs() const;

      void setOnceEnabled( bool e );
      bool getOnceEnabled() const;

      void setRandomPhaseEnabled( bool e );
      bool getRandomPhaseEnabled() const;

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
      bool m_SyncEnabled;
      double m_SyncBeats;

      bool m_DelaySyncEnabled;
      double m_DelaySecs;
      double m_DelayBeats;

      bool m_FadeInSyncEnabled;
      double m_FadeInSecs;
      double m_FadeInBeats;

      bool m_OnceEnabled;
      bool m_RandomPhaseEnabled;
   };
}
#endif

