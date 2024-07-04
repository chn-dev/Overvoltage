/*----------------------------------------------------------------------------*/
/*!
\file Part.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class Part.
*/
/*----------------------------------------------------------------------------*/
#ifndef __PART_H__
#define __PART_H__

#include <list>
#include <map>
#include <libxml/tree.h>

#include "Sample.h"
#include "Voice.h"

//==============================================================================
namespace SamplerEngine
{
   class OutputBus;
   class Engine;

   /*----------------------------------------------------------------------------*/
   /*!
   \class Part
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class Part
   {
   public:
      Part( size_t partNum, Engine *pEngine  = nullptr );
      ~Part();

      void setEngine( Engine *pEngine );

      void noteOn( int note, int vel );
      void noteOff( int note, int vel );
      void setPitchbend( double v );
      double getPitchbend() const;
      void setController( int ccNum, double v );
      double getController( int ccNum ) const;

      std::list<Sample *> &samples();
      const std::list<Sample *> &constSamples() const;

      bool containsSample( const Sample *pSample ) const;

      size_t getPartNum() const;
      void setPartNum( size_t n );
      void deleteSample( Sample *pSample );
      void removeSample( Sample *pSample );
      void addSample( Sample *pSample );

      bool isPlaying( const Sample *pSample ) const;


      static Part *fromXml( xmlNode *pe );
      xmlNode *toXml() const;

      bool process( std::vector<OutputBus> &buses, double sampleRate, double bpm );

   private:
      std::list<Sample *> getSamplesByMidiNoteAndVelocity( int note, int vel ) const;
      void stopVoice( const Voice *pVoice );
      void stopAllVoices();

   private:
      size_t m_PartNum;
      Engine *m_pEngine;
      double m_Pitchbend;
      std::map<int, double> m_ControllerValues;
      std::list<Sample *> m_Samples;
      std::multimap<int, Voice *> m_Voices;
   };
}

#endif

