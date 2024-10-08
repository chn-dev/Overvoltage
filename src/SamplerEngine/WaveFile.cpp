/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of chn's Overvoltage.                                   *
 *                                                                             *
 *  Overvoltage is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *
 *  Overvoltage is distributed in the hope that it will be useful, but         *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Overvoltage. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file WaveFile.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a representation of a wave file
*/
/*----------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
#include <string>

#include <DSP/DFT.h>

#include "WaveFile.h"
#include "util.h"

using namespace SamplerEngine;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
WaveFile::~WaveFile()
{
   if( m_pData )
   {
      delete m_pData;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Create an XML element from the WaveFile.
\return Pointer to the new XML element
*/
/*----------------------------------------------------------------------------*/
xmlNode *WaveFile::toXml() const
{
   xmlNode *pe = xmlNewNode( nullptr, (xmlChar *)"wave" );

   xmlNode *peNChannels = xmlNewNode( nullptr, (xmlChar *)"nchannels" );
   xmlAddChild( peNChannels, xmlNewText( (xmlChar * )stdformat( "{}", m_nChannels ).c_str() ) );
   xmlAddChild( pe, peNChannels );

   xmlNode *peSampleRate = xmlNewNode( nullptr, (xmlChar *)"samplerate" );
   xmlAddChild( peSampleRate, xmlNewText( (xmlChar *)stdformat( "{}", m_SampleRate ).c_str() ) );
   xmlAddChild( pe, peSampleRate );

   xmlNode *peNBits = xmlNewNode( nullptr, (xmlChar *)"nbits" );
   xmlAddChild( peNBits, xmlNewText( (xmlChar * )stdformat( "{}", m_nBits ).c_str() ) );
   xmlAddChild( pe, peNBits );

   xmlNode *peNSamples = xmlNewNode( nullptr, (xmlChar *)"nsamples" );
   xmlAddChild( peNSamples, xmlNewText( (xmlChar *)stdformat( "{}", m_nSamples ).c_str() ) );
   xmlAddChild( pe, peNSamples );

   xmlNode *peLoopStart = xmlNewNode( nullptr, (xmlChar *)"loopstart" );
   xmlAddChild( peLoopStart, xmlNewText( (xmlChar *)stdformat( "{}", m_LoopStart ).c_str() ) );
   xmlAddChild( pe, peLoopStart );

   xmlNode *peLoopEnd = xmlNewNode( nullptr, (xmlChar *)"loopend" );
   xmlAddChild( peLoopEnd, xmlNewText( (xmlChar *)stdformat( "{}", m_LoopEnd ).c_str() ) );
   xmlAddChild( pe, peLoopEnd );

   xmlNode *peIsLooped = xmlNewNode( nullptr, (xmlChar *)"islooped" );
   xmlAddChild( peIsLooped, xmlNewText( (xmlChar *)( m_IsLooped ? "true" : "false" ) ) );
   xmlAddChild( pe, peIsLooped );

   xmlNode *peData = xmlNewNode( nullptr, (xmlChar *)"data" );
   std::vector<uint8_t> data;
   size_t dataSize = (size_t)m_nChannels * (size_t)m_nBits * (size_t)m_nSamples / 8;
   data.resize( dataSize );
   memcpy( data.data(), m_pData, dataSize );
   xmlAddChild( peData, xmlNewText( (xmlChar *)util::base64encode( data ).c_str() ) );
   xmlAddChild( pe, peData );

   return( pe );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Reconstruct a Wavefile object from a previously generated XML element (see toXml()).
\param pe The XML element
\return Pointer to the WaveFile object or nullptr on error
*/
/*----------------------------------------------------------------------------*/
WaveFile *WaveFile::fromXml( xmlNode *pe )
{
   if( std::string( (char*)pe->name ) != "wave" )
      return( nullptr );

   int nChannels = -1;
   int sampleRate = -1;
   int nBits = -1;
   uint32_t nSamples = ~(decltype( nSamples ))0;
   uint32_t loopStart = ~(decltype( loopStart ))0;
   uint32_t loopEnd = ~(decltype( loopEnd ))0;
   bool isLooped = false;
   uint8_t *pData = nullptr;

   for( xmlNode *pChild = pe->children; pChild; pChild = pChild->next )
   {
      std::string tagName = std::string( (char*)pChild->name );

      if( tagName == "nchannels" )
      {
         nChannels = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "samplerate" )
      {
         sampleRate = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "nbits" )
      {
         nBits = std::stoi( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "nsamples" )
      {
         nSamples = std::stoul( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "loopstart" )
      {
         loopStart = std::stoul( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "loopend" )
      {
         loopEnd = std::stoul( std::string( (char*)pChild->children->content ) );
      } else
      if( tagName == "islooped" )
      {
         std::string v = std::string( (char*)pChild->children->content );
         isLooped = ( v == "true" );
      } else
      if( tagName == "data" )
      {
         std::string v = std::string( (char*)pChild->children->content );
         std::vector<uint8_t> d = util::base64decode( v );
         size_t dataSize = d.size();
         pData = new uint8_t[dataSize];
         memcpy( pData, d.data(), dataSize );
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

      pWaveFile->m_ToFloatLambdaFunction = pWaveFile->getToFloatLambdaFunction();

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


/*----------------------------------------------------------------------------*/
/*! 2024-09-04
\return A lambda function
  []( const WaveFile, int nChannel, uint32_t nSample )
  for reading a float sample value from a WaveFile object.
*/
/*----------------------------------------------------------------------------*/
std::function<float( const WaveFile *, int, uint32_t )> WaveFile::getToFloatLambdaFunction() const
{

   if( numBits() == 16 )
   {
      if( numChannels() == 1 )
      {
         return(
            []( const WaveFile *pWav, int /*nChan*/, uint32_t nSample ) -> float
            {
               uint32_t o = 2 * nSample;
               uint8_t *pData = pWav->data8();
               int16_t v = pData[o + 0] | ( pData[o + 1] << 8 );
               return( (float)v / 32768.0 );
            }
         );
      } else
      if( numChannels() == 2 )
      {
         return(
            []( const WaveFile *pWav, int nChan, uint32_t nSample ) -> float
            {
               uint32_t o = ( 4 * nSample ) + ( 2 * (uint32_t)nChan );
               uint8_t *pData = pWav->data8();
               int16_t v = pData[o + 0] | ( pData[o + 1] << 8 );
               return( (float)v / 32768.0 );
            }
         );
      }
   } else
   if( numBits() == 8 )
   {
      if( numChannels() == 1 )
      {
         return(
            []( const WaveFile *pWav, int /*nChan*/, uint32_t nSample ) -> float
            {
               return( (float)pWav->data8()[nSample] / 128.0 );
            }
         );
      } else
      if( numChannels() == 2 )
      {
         return(
            []( const WaveFile *pWav, int nChan, uint32_t nSample ) -> float
            {
               uint32_t o = ( 2 * nSample ) + (uint32_t)nChan;
               uint8_t *pData = pWav->data8();
               int8_t v = (int8_t)pData[o];
               return( (float)v / 128.0 );
            }
         );
      }
   }

   return( nullptr );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Size in bytes of the WaveFile
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::size() const
{
   return( m_nSamples * m_nChannels * ( m_nBits / 8 ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Sample number of the loop start
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::loopStart() const
{
   return( m_LoopStart );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Sample number of the loop end
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::loopEnd() const
{
   return( m_LoopEnd );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param v Sample number of the loop start
*/
/*----------------------------------------------------------------------------*/
void WaveFile::setLoopStart( uint32_t v )
{
   m_LoopStart = v;
   if( m_LoopStart & 0x80000000 )
      m_LoopStart = 0;
   else
   if( m_LoopStart >= m_nSamples )
      m_LoopStart = m_nSamples - 1;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param v Sample number of the loop end
*/
/*----------------------------------------------------------------------------*/
void WaveFile::setLoopEnd( uint32_t v )
{
   m_LoopEnd = v;
   if( m_LoopEnd & 0x80000000 )
      m_LoopEnd = 0;
   else
   if( m_LoopEnd >= m_nSamples )
      m_LoopEnd = m_nSamples - 1;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return true if the sample ought to be looped
*/
/*----------------------------------------------------------------------------*/
bool WaveFile::isLooped() const
{
   return( m_IsLooped );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Number of channels
*/
/*----------------------------------------------------------------------------*/
int WaveFile::numChannels() const
{
   return( m_nChannels );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Sample rate in Hz
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::sampleRate() const
{
   return( m_SampleRate );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return Number of bits per sample
*/
/*----------------------------------------------------------------------------*/
int WaveFile::numBits() const
{
   return( m_nBits );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return 16bit pointer to the data
*/
/*----------------------------------------------------------------------------*/
uint16_t *WaveFile::data16() const
{
   return( (uint16_t *)m_pData );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return 8bit pointer to the data
*/
/*----------------------------------------------------------------------------*/
uint8_t *WaveFile::data8() const
{
   return( (uint8_t *)m_pData );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Retrieve a single sample as a floating point value.
\param nChannel Channel number
\param nSample The sample number
\return The floating point number or NAN on error
*/
/*----------------------------------------------------------------------------*/
float WaveFile::floatValue( int nChannel, uint32_t nSample ) const
{
   return( m_ToFloatLambdaFunction( this, nChannel, nSample ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
std::string WaveFile::readTagName( std::ifstream &file )
{
   char tmp[5];

   tmp[4] = '\0';
   if( file.read( (char *)tmp, 4 ).eof() )
      return( "" );

   return( std::string( tmp ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Read the next four bytes from a file and return the appropriate 32bit value.
\param file The input file
\return The 32bit value
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::readDWord( std::ifstream &file )
{
   unsigned char tmpDW[4];

   file.read( (char *)&tmpDW, sizeof( tmpDW ) );
   return( ( (uint32_t)tmpDW[3] << 24 ) |
           ( (uint32_t)tmpDW[2] << 16 ) |
           ( (uint32_t)tmpDW[1] << 8 ) |
           ( (uint32_t)tmpDW[0] ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pD Pointer to some memory
\return A 16bit value retrieved from the first 2 8bit-values of the memory
*/
/*----------------------------------------------------------------------------*/
uint16_t WaveFile::getWord( const unsigned char *pD )
{
   return( pD[0] | pD[1] << 8 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pD Pointer to some memory
\return A 32bit value retrieved from the first 4 8bit-values of the memory
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::getDWord( const unsigned char *pD )
{
   return( ( (uint32_t)pD[0] ) | ( (uint32_t)pD[1] << 8 ) | ( (uint32_t)pD[2] << 16 ) | ( (uint32_t)pD[3] << 24 ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Load a wave from a file.
\param fname The file name
\return Pointer to the new WaveFile or nullptr on error
*/
/*----------------------------------------------------------------------------*/
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
         delete[] pFmt;

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

   pWav->m_ToFloatLambdaFunction = pWav->getToFloatLambdaFunction();

   return( pWav );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The number of samples
*/
/*----------------------------------------------------------------------------*/
uint32_t WaveFile::numSamples() const
{
   return( m_nSamples );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void WaveFile::dft() const
{
   std::vector<DSP::Complex> d;

   /*bool ok = */DSP::DFT::dft( *this, 0, 0, 512, d, DSP::DFT::WindowHamming );

//   double p = d[0].getA() * d[0].getA();
//   double pdb = 10.0 * log10( p );
   std::vector<double> db;
   for( size_t i = 0; i < d.size(); i++ )
   {
      //db.push_back( log10( d[i].getR() / (double)(d.size()/2) ) );
      db.push_back( 10.0 * log10( sqrt( d[i].getA() * d[i].getA() + d[i].getB() * d[i].getB() ) ) );
   }
}

