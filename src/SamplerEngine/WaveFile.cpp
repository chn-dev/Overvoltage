#include <math.h>
#include <string>

#include "WaveFile.h"
#include "util.h"

using namespace SamplerEngine;

WaveFile::WaveFile() :
   m_Format( (decltype( m_Format ))-1 ),
   m_nChannels( (decltype( m_nChannels ))-1 ),
   m_SampleRate( ~(decltype( m_SampleRate ))0 ),
   m_nBits( (decltype( m_nBits ))-1 ),
   m_nSamples( ~(decltype( m_nSamples ))0 ),
   m_LoopStart( ~(decltype( m_LoopStart ))0 ),
   m_LoopEnd( ~(decltype( m_LoopEnd ))0 ),
   m_IsLooped( false ),
   m_pData( nullptr )
{
}


WaveFile::~WaveFile()
{
   if( m_pData )
   {
      delete m_pData;
   }
}


juce::XmlElement *WaveFile::toXml() const
{
   juce::XmlElement *pe = new juce::XmlElement( "wave" );

   juce::XmlElement *peNChannels = new juce::XmlElement( "nchannels" );
   peNChannels->addTextElement( stdformat( "{}", m_nChannels ) );
   pe->addChildElement( peNChannels );

   juce::XmlElement *peSampleRate = new juce::XmlElement( "samplerate" );
   peSampleRate->addTextElement( stdformat( "{}", m_SampleRate ) );
   pe->addChildElement( peSampleRate );

   juce::XmlElement *peNBits = new juce::XmlElement( "nbits" );
   peNBits->addTextElement( stdformat( "{}", m_nBits ) );
   pe->addChildElement( peNBits );

   juce::XmlElement *peNSamples = new juce::XmlElement( "nsamples" );
   peNSamples->addTextElement( stdformat( "{}", m_nSamples ) );
   pe->addChildElement( peNSamples );

   juce::XmlElement *peLoopStart = new juce::XmlElement( "loopstart" );
   peLoopStart->addTextElement( stdformat( "{}", m_LoopStart ) );
   pe->addChildElement( peLoopStart );

   juce::XmlElement *peLoopEnd = new juce::XmlElement( "loopend" );
   peLoopEnd->addTextElement( stdformat( "{}", m_LoopEnd ) );
   pe->addChildElement( peLoopEnd );

   juce::XmlElement *peIsLooped = new juce::XmlElement( "islooped" );
   peIsLooped->addTextElement( m_IsLooped ? "true" : "false" );
   pe->addChildElement( peIsLooped );

   juce::XmlElement *peData = new juce::XmlElement( "data" );
   juce::MemoryBlock mb = juce::MemoryBlock( m_pData, (uint32_t)m_nChannels * (uint32_t)m_nBits * m_nSamples / 8 );
   peData->addTextElement( mb.toBase64Encoding() );
   pe->addChildElement( peData );

   return( pe );
}


WaveFile *WaveFile::fromXml( const juce::XmlElement *pe )
{
   if( pe->getTagName() != "wave" )
      return( nullptr );

   int nChannels = -1;
   int sampleRate = -1;
   int nBits = -1;
   uint32_t nSamples = ~(decltype( nSamples ))0;
   uint32_t loopStart = ~(decltype( loopStart ))0;
   uint32_t loopEnd = ~(decltype( loopEnd ))0;
   bool isLooped = false;
   uint8_t *pData = nullptr;

   for( int i = 0; pe->getChildElement( i ); i++ )
   {
      juce::XmlElement *pChild = pe->getChildElement( i );
      std::string tagName = pChild->getTagName().toStdString();

      if( tagName == "nchannels" )
      {
         nChannels = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "samplerate" )
      {
         sampleRate = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "nbits" )
      {
         nBits = std::stoi( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "nsamples" )
      {
         nSamples = std::stoul( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "loopstart" )
      {
         loopStart = std::stoul( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "loopend" )
      {
         loopEnd = std::stoul( pChild->getChildElement( 0 )->getText().toStdString() );
      } else
      if( tagName == "islooped" )
      {
         std::string v = pChild->getChildElement( 0 )->getText().toStdString();
         isLooped = ( v == "true" );
      } else
      if( tagName == "data" )
      {
         std::string v = pChild->getChildElement( 0 )->getText().toStdString();
         juce::MemoryBlock mem;
         if( mem.fromBase64Encoding( v ) )
         {
            size_t dataSize = mem.getSize();
            pData = new uint8_t[dataSize];
            memcpy( pData, mem.getData(), dataSize );
         }
      }
   }

   if( nChannels >= 0 && sampleRate >= 0 &&
       nBits >= 0 && nSamples != ~(decltype( nSamples ))0 &&
       loopStart != ~(decltype( loopStart ))0 && loopEnd != ~(decltype( loopEnd ))0 && pData )
   {
      WaveFile *pWaveFile = new WaveFile();
      pWaveFile->m_Format = 1;
      pWaveFile->m_nChannels = (uint16_t)nChannels;
      pWaveFile->m_SampleRate = (uint32_t)sampleRate;
      pWaveFile->m_nBits = (uint16_t)nBits;
      pWaveFile->m_nSamples = nSamples;
      pWaveFile->m_LoopStart = loopStart;
      pWaveFile->m_LoopEnd = loopEnd;
      pWaveFile->m_IsLooped = isLooped;
      pWaveFile->m_pData = pData;

      return( pWaveFile );
   } else
   {
      if( pData )
      {
         free( pData );
      }
      return( nullptr );
   }
}


uint32_t WaveFile::size() const
{
   return( m_nSamples * m_nChannels * ( m_nBits / 8 ) );
}


uint32_t WaveFile::loopStart() const
{
   return( m_LoopStart );
}


uint32_t WaveFile::loopEnd() const
{
   return( m_LoopEnd );
}


void WaveFile::setLoopStart( uint32_t v )
{
   m_LoopStart = v;
   if( m_LoopStart & 0x80000000 )
      m_LoopStart = 0;
   else
   if( m_LoopStart >= m_nSamples )
      m_LoopStart = m_nSamples - 1;
}


void WaveFile::setLoopEnd( uint32_t v )
{
   m_LoopEnd = v;
   if( m_LoopEnd & 0x80000000 )
      m_LoopEnd = 0;
   else
   if( m_LoopEnd >= m_nSamples )
      m_LoopEnd = m_nSamples - 1;
}


bool WaveFile::isLooped() const
{
   return( m_IsLooped );
}


int WaveFile::numChannels() const
{
   return( m_nChannels );
}


uint32_t WaveFile::sampleRate() const
{
   return( m_SampleRate );
}


int WaveFile::numBits() const
{
   return( m_nBits );
}


uint16_t *WaveFile::data16() const
{
   return( (uint16_t *)m_pData );
}


uint8_t *WaveFile::data8() const
{
   return( (uint8_t *)m_pData );
}


float WaveFile::floatValue( int nChannel, uint32_t nSample ) const
{
   if( (uint32_t)nChannel >= m_nChannels )
      return( NAN );

   if( nSample >= m_nSamples )
      return( NAN );

   if( m_nBits == 16 )
   {
      int16_t v = (int16_t)data16()[( nSample * m_nChannels ) + (uint32_t)nChannel];
      return( (float)v / 32768.0f );
   } else
   if( m_nBits == 8 )
   {
      uint8_t v = data8()[( nSample * m_nChannels ) + (uint32_t)nChannel];
      return( (float)v / 128 );
   } else
   {
      return( NAN );
   }
}


std::string WaveFile::readTagName( std::ifstream &file )
{
   char tmp[5];

   tmp[4] = '\0';
   if( file.read( (char *)tmp, 4 ).eof() )
      return( "" );

   return( std::string( tmp ) );
}


uint32_t WaveFile::readDWord( std::ifstream &file )
{
   unsigned char tmpDW[4];

   file.read( (char *)&tmpDW, sizeof( tmpDW ) );
   return( ( (uint32_t)tmpDW[3] << 24 ) |
           ( (uint32_t)tmpDW[2] << 16 ) |
           ( (uint32_t)tmpDW[1] << 8 ) |
           ( (uint32_t)tmpDW[0] ) );
}


uint16_t WaveFile::getWord( const unsigned char *pD )
{
   return( pD[0] | pD[1] << 8 );
}


uint32_t WaveFile::getDWord( const unsigned char *pD )
{
   return( ( (uint32_t)pD[0] ) | ( (uint32_t)pD[1] << 8 ) | ( (uint32_t)pD[2] << 16 ) | ( (uint32_t)pD[3] << 24 ) );
}


WaveFile *WaveFile::load( std::string fname )
{
   std::ifstream file;

   file.open( fname, std::ios_base::binary );
   if( !file )
      return( nullptr );

   WaveFile *pWav = new WaveFile();

   file.seekg( 0, std::ios_base::beg );
   if( readTagName( file ) !=  "RIFF" )
   {
      file.close();
      return( nullptr );
   }

   /*uint32_t riffLen = */readDWord( file );

   if( readTagName( file ) != "WAVE" )
   {
      file.close();
      return( nullptr );
   }

   bool haveFormat = false;
   bool haveData = false;
   bool ok = true;
   while( true )
   {
      std::string tagName = readTagName( file );
      uint32_t tagLen = readDWord( file );
      uint32_t tagRead = 0;

      if( tagName == "fmt " )
      {
         if( tagLen < 16 )
         {
            ok = false;
            break;
         }

         unsigned char *pFmt = new unsigned char[tagLen];
         file.read( (char *)pFmt, tagLen );

         pWav->m_Format = getWord( pFmt + 0 );
         pWav->m_nChannels = getWord( pFmt + 2 );
         pWav->m_SampleRate = getDWord( pFmt + 4 );
         pWav->m_nBits = getWord( pFmt + 14 );
         delete pFmt;

         if( pWav->m_Format != 1 )
         {
            ok = false;
            break;
         }

         haveFormat = true;
         tagRead = tagLen;
      } else
      if( tagName == "smpl" )
      {
         if( tagLen >= 60 )
         {
            unsigned char *pFmt = new unsigned char[tagLen];
            file.read( (char *)pFmt, tagLen );
            tagRead = tagLen;

            uint32_t nSampleLoops = getDWord( pFmt + 0x24 - 8 );
            if( nSampleLoops > 0 )
            {
//               uint32_t id = getDWord( pFmt + ( i * 6 * 4 ) + 0x2c - 8 );
               uint32_t loopStart = getDWord( pFmt + ( 0 * 6 * 4 ) + 0x2c - 8 + 8 );
               uint32_t loopEnd   = getDWord( pFmt + ( 0 * 6 * 4 ) + 0x2c - 8 + 12 ) - 1;

               pWav->m_LoopStart = loopStart;
               pWav->m_LoopEnd = loopEnd;
               pWav->m_IsLooped = true;
            }
         }
      } else
      if( tagName == "data" )
      {
         uint8_t *pData = new uint8_t[tagLen];
         file.read( (char *)pData, tagLen );
         pWav->m_pData = pData;
         haveData = true;
         pWav->m_nSamples = tagLen;
         tagRead = tagLen;
      }

      file.seekg( tagLen - tagRead, std::ios_base::cur );

      if( file.eof() || ( haveData && haveFormat ) )
         break;
   }

   file.close();

   if( !ok )
   {
      delete pWav;
      pWav = nullptr;
   }

   pWav->m_nSamples = pWav->m_nSamples / ( (uint32_t)pWav->m_nChannels * ( (uint32_t)pWav->m_nBits / 8 ) );
   if( !pWav->isLooped() )
   {
      pWav->m_LoopStart = 0;
      pWav->m_LoopEnd = pWav->m_nSamples - 1;
   }

   return( pWav );
}


uint32_t WaveFile::numSamples() const
{
   return( m_nSamples );
}
