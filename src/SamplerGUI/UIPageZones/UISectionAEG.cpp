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

