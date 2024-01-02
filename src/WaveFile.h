#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "JuceHeader.h"

//==============================================================================
class WaveFile
{
public:
   ~WaveFile();

   static WaveFile *load( std::string fname );

   int numChannels() const;
   uint32_t sampleRate() const;
   int numBits() const;
   uint32_t numSamples() const;

   uint32_t loopStart() const;
   uint32_t loopEnd() const;
   void setLoopStart( uint32_t v );
   void setLoopEnd( uint32_t v );
   bool isLooped() const;

   uint16_t *data16() const;
   uint8_t *data8() const;

   float floatValue( int nChannel, int nSample ) const;

   uint32_t size() const;

   juce::XmlElement *getStateInformation() const;
   static WaveFile *fromXml( const juce::XmlElement *pe );

protected:
   static std::string readTagName( std::ifstream &file );
   static uint32_t readDWord( std::ifstream &file );
   static uint16_t getWord( const unsigned char *pD );
   static uint16_t getDWord( const unsigned char *pD );

private:
   WaveFile();

private:
   uint16_t m_Format;
   uint16_t m_nChannels;
   uint32_t m_SampleRate;
   uint16_t m_nBits;
   uint32_t m_nSamples;
   uint32_t m_LoopStart;
   uint32_t m_LoopEnd;
   bool m_IsLooped;
   void *m_pData;
};
