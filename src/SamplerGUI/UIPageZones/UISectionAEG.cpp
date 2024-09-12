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
\file UISectionAEG.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the AEG UI section
*/
/*----------------------------------------------------------------------------*/
#include "UISectionAEG.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionAEG::UISectionAEG( UIPage *pUIPage ) :
   UISectionEG( pUIPage, "AEG" )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionAEG::~UISectionAEG()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample A sample
\return The samples AEG
*/
/*----------------------------------------------------------------------------*/
SamplerEngine::ENV *UISectionAEG::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getAEG() );
}

