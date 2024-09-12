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
\file UISectionAEG.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionAEG.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONAEG_H__
#define __UISECTIONAEG_H__

#include "JuceHeader.h"

#include "UISectionEG.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionAEG
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionAEG : public UISectionEG
   {
   public:
      UISectionAEG( UIPage *pUIPage );
      ~UISectionAEG();

   protected:
      virtual SamplerEngine::ENV *getENV( const SamplerEngine::Sample *pSample ) const;
   };
}

#endif

