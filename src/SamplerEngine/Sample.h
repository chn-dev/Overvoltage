/*----------------------------------------------------------------------------*/
/*!
\file Sample.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class Sample.
*/
/*----------------------------------------------------------------------------*/
#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <set>
#include <vector>
#include <libxml/tree.h>

#include "WaveFile.h"
#include "ENV.h"
#include "LFO.h"
#include "Filter.h"
#include "ModMatrix.h"

#define NUM_LFO 3

//==============================================================================
namespace SamplerEngine
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class Sample
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
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

      Sample( std::string name, WaveFile *pWave, int minNote, int maxNote, int nLayer );
      ~Sample();

      static Sample *fromXml( xmlNode *pe );
      xmlNode *toXml() const;

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
      float getKeytrack() const;
      void setKeytrack( float kt );
      float getPitchbendRange() const;
      void setPitchbendRange( float pb );
      Sample::PlayMode getPlayMode() const;
      ENV *getAEG() const;
      ENV *getEG2() const;
      LFO *getLFO( size_t n ) const;
      size_t getNumLFOs() const;
      Filter *getFilter() const;
      ModMatrix *getModMatrix() const;
      int getOutputBus() const;
      void setOutputBus( int n );

      static std::string toString( PlayMode mode );
      static PlayMode fromString( std::string mode );
      static std::set<PlayMode> allPlayModes();

      void setLayer( int nLayer );
      int getLayer() const;

   protected:

   private:
      Sample();

      std::string m_Name;
      int m_OutputBus;
      ENV *m_pAEG;
      ENV *m_pEG2;
      std::vector<LFO *> m_LFOs;
      LFO *m_pLFO2;
      WaveFile *m_pWave;
      PlayMode m_PlayMode;
      Filter *m_pFilter;
      ModMatrix *m_pModMatrix;
      float m_DetuneCents;
      float m_Pan;
      float m_Gain;
      float m_Keytrack;
      float m_PitchbendRange;
      bool m_Reverse;
      int m_BaseNote;
      int m_MinNote;
      int m_MaxNote;
      int m_MinVelocity;
      int m_MaxVelocity;
      int m_NLayer;
   };
}

#endif
