#pragma once

#include <set>

#include "WaveFile.h"
#include "ENV.h"

//==============================================================================
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
   Sample::PlayMode getPlayMode() const;
   ENV *getAEG() const;

   static std::string toString( PlayMode mode );
   static PlayMode fromString( std::string mode );
   static std::set<PlayMode> allPlayModes();

protected:

private:
   std::string m_Name;
   ENV *m_pAEG;
   WaveFile *m_pWave;
   PlayMode m_PlayMode;
   float m_DetuneCents;
   bool m_Reverse;
   int m_BaseNote;
   int m_MinNote;
   int m_MaxNote;
   int m_MinVelocity;
   int m_MaxVelocity;
};
