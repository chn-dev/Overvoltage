#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <set>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "WaveFile.h"
#include "ENV.h"
#include "Filter.h"

//==============================================================================
namespace SamplerEngine
{
   class Sample
   {
   public:
      enum PlayMode
      {
         PlayModeStandard = 1,
         PlayModeLoop,
         PlayModeShot,
         PlayModeLoopUntilRelease
      };

      Sample( std::string name, WaveFile *pWave, int minNote, int maxNote );
      ~Sample();

      static Sample *fromXml( const juce::XmlElement *pe );
      juce::XmlElement *toXml() const;

      std::string getName() const;
      void setName( std::string name );
      WaveFile *getWave() const;
      int getBaseNote() const;
      void setBaseNote( int note );
      int getMinNote() const;
      int getMaxNote() const;
      void setMinNote( int note );
      void setMaxNote( int note );
      void correctMinMaxNote();
      bool matchesMidiNote( int note ) const;
      bool matchesVelocity( int vel ) const;
      float getDetune() const;
      void setDetune( float d );
      int getMinVelocity() const;
      void setMinVelocity( int v );
      int getMaxVelocity() const;
      void setMaxVelocity( int v );
      bool getReverse() const;
      void setReverse( bool r );
      void setPlayMode( Sample::PlayMode pm );
      float getPan() const;
      void setPan( float pan );
      float getGain() const;
      void setGain( float gain );
      Sample::PlayMode getPlayMode() const;
      ENV *getAEG() const;
      ENV *getEG2() const;
      Filter *getFilter() const;
      int getOutputBus() const;
      void setOutputBus( int n );

      static std::string toString( PlayMode mode );
      static PlayMode fromString( std::string mode );
      static std::set<PlayMode> allPlayModes();

   protected:

   private:
      Sample();

      std::string m_Name;
      int m_OutputBus;
      ENV *m_pAEG;
      ENV *m_pEG2;
      WaveFile *m_pWave;
      PlayMode m_PlayMode;
      Filter *m_pFilter;
      float m_DetuneCents;
      float m_Pan;
      float m_Gain;
      bool m_Reverse;
      int m_BaseNote;
      int m_MinNote;
      int m_MaxNote;
      int m_MinVelocity;
      int m_MaxVelocity;
   };
}

#endif
