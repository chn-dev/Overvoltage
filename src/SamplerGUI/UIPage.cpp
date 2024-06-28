/*----------------------------------------------------------------------------*/
/*!
\file UIPage.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a UI page
*/
/*----------------------------------------------------------------------------*/
#include "UIPage.h"
#include "UIPageZones/UISectionSamplerKeyboard.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UIPage::UIPage( PluginEditor *pEditor ) :
   m_pEditor( pEditor )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UIPage::~UIPage()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UIPage::paint( juce::Graphics &/*g*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UIPage::resized()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSection
*/
/*----------------------------------------------------------------------------*/
void UIPage::addUISection( SamplerGUI::UISection *pSection )
{
   if( pSection )
   {
      m_UISections.insert( pSection );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\param pSection
*/
/*----------------------------------------------------------------------------*/
void UIPage::removeUISection( SamplerGUI::UISection *pSection )
{
   if( pSection && ( m_UISections.find( pSection ) != m_UISections.end() ) )
   {
      m_UISections.erase( pSection );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
This is called when the user has (de-)selected samples.
\param pKeyboard The sampler keyboard
*/
/*----------------------------------------------------------------------------*/
void UIPage::onSampleSelectionUpdated( UISectionSamplerKeyboard *pKeyboard )
{
   for( UISection *pSection : m_UISections )
   {
      pSection->onSampleSelectionUpdated( pKeyboard );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Update all UI sections
*/
/*----------------------------------------------------------------------------*/
void UIPage::updateUISections()
{
   for( SamplerGUI::UISection *pSection : m_UISections )
   {
      pSection->samplesUpdated();
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return The plugin editor
*/
/*----------------------------------------------------------------------------*/
PluginEditor *UIPage::editor() const
{
   return( m_pEditor );
}

