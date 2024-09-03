/*----------------------------------------------------------------------------*/
/*!
\file WaveFile.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class WaveFile.
*/
/*----------------------------------------------------------------------------*/
#ifndef __WAVEFILE_H__
#define __WAVEFILE_H__

#include <string>
#include <iostream>
#include <fstream>
#include <functional>

#include <libxml/tree.h>

#include <DSP/Wave.h>

//==============================================================================
namespace SamplerEngine
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class WaveFile
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class WaveFile : public DSP::Wave
   {
   public:
      virtual ~WaveFile();

      static WaveFile *load( std::string fname );

      uint32_t loopStart() const;
      uint32_t loopEnd() const;
      void setLoopStart( uint32_t v );
      void setLoopEnd( uint32_t v );
      bool isLooped() const;

      uint16_t *data16() const;
      uint8_t *data8() const;

      virtual float floatValue( int nChannel, uint32_t nSample ) const;
      virtual int numChannels() const;
      virtual uint32_t sampleRate() const;
      virtual int numBits() const;
      virtual uint32_t numSamples() const;

      std::function<float( const WaveFile *, int, uint32_t )> getToFloatLambdaFunction() const;

      void dft() const;

      uint32_t size() const;

      static WaveFile *fromXml( xmlNode *pe );
      xmlNode *toXml() const;

   protected:
      static std::string readTagName( std::ifstream &file );
      static uint32_t readDWord( std::ifstream &file );
      static uint16_t getWord( const unsigned char *pD );
      static uint32_t getDWord( const unsigned char *pD );

   private:
      WaveFile();

   private:
      std::function<float( const WaveFile *, int, uint32_t )> m_ToFloatLambdaFunction;

      uint16_t m_Format;
      uint16_t m_nChannels;
      uint32_t m_SampleRate;
      uint16_t m_nBits;
      uint32_t m_nSamples;
      uint32_t m_LoopStart;
      uint32_t m_LoopEnd;
      bool m_IsLooped;
      uint8_t *m_pData;
   };
}

#endif

