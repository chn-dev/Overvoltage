/*----------------------------------------------------------------------------*/
/*!
\file UISection.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the EG2 UI section
*/
/*----------------------------------------------------------------------------*/
#include "UISectionEG2.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionEG2::UISectionEG2( UIPage *pUIPage ) :
   UISectionEG( pUIPage, "EG2" )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionEG2::~UISectionEG2()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSample A sample
\return The sample's EG2 envelope generator
*/
/*----------------------------------------------------------------------------*/
SamplerEngine::ENV *UISectionEG2::getENV( const SamplerEngine::Sample *pSample ) const
{
   return( pSample->getEG2() );
}

