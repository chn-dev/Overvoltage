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
\file SamplerEngine.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class Engine.
*/
/*----------------------------------------------------------------------------*/
#ifndef __SAMPLERENGINE_H__
#define __SAMPLERENGINE_H__

#include "Part.h"
#include "Voice.h"

#include <libxml/tree.h>

#define SAMPLERENGINE_NUMLAYERS 8
#define SAMPLERENGINE_NUMPARTS 16

class PluginProcessor;

namespace SamplerEngine
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class OutputBus
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class OutputBus
   {
   public:
      OutputBus();
      OutputBus( size_t numSamples, std::vector<float *> writePointers );
      ~OutputBus();

      size_t getNumSamples() const;
      std::vector<float *> &getWritePointers();
      bool isValid() const;

   private:
      bool m_Valid;
      size_t m_NumSamples;
      std::vector<float *> m_WritePointers;
   };

   /*----------------------------------------------------------------------------*/
   /*!
   \class Engine
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class Engine
   {
   public:
      Engine( PluginProcessor *pProcessor = nullptr );
      ~Engine();

      bool process( std::vector<OutputBus> &buses, double sampleRate, double bpm );

      void setProcessor( PluginProcessor *pProcessor );

      Part *findPart( const Sample *pSample );
      Part *getPart( size_t nPart );

      void deleteSample( size_t part, Sample *pSample );
      std::list<Sample *> samples( size_t nPart ) const;

      void noteOn( size_t nPart, int note, int vel );
      void noteOff( size_t nPart, int note, int vel );
      void pitchbend( size_t nPart, double v );
      void controllerChange( size_t nPart, int ccNum, double v );

      std::list<Sample *> &samples( size_t nPart );
      const std::list<Sample *> &constSamples( size_t nPart ) const;

      bool isSoloEnabled() const;
      std::set<Sample *> getSelectedSamples() const;

      bool isPlaying( size_t nPart, const Sample *pSample ) const;

      void importPart( size_t nPart, xmlNode *pXmlPart );

      xmlNode *toXml() const;
      static Engine *fromXml( xmlNode *peOvervoltage );

   private:
      PluginProcessor *m_pProcessor;
      std::vector<Part *> m_Parts;
   };
}

#endif

