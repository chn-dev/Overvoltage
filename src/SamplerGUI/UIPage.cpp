#include "UIPage.h"
#include "UIPageZones/UISectionSamplerKeyboard.h"

using namespace SamplerGUI;

UIPage::UIPage( PluginEditor *pEditor ) :
   m_pEditor( pEditor )
{
}


UIPage::~UIPage()
{
}


void UIPage::paint( juce::Graphics &/*g*/ )
{
}


void UIPage::resized()
{
}


void UIPage::addUISection( SamplerGUI::UISection *pSection )
{
   if( pSection )
   {
      m_UISections.insert( pSection );
   }
}


void UIPage::removeUISection( SamplerGUI::UISection *pSection )
{
   if( pSection && ( m_UISections.find( pSection ) != m_UISections.end() ) )
   {
      m_UISections.erase( pSection );
   }
}


void UIPage::onSampleSelectionUpdated( UISectionSamplerKeyboard *pKeyboard )
{
   for( UISection *pSection : m_UISections )
   {
      pSection->onSampleSelectionUpdated( pKeyboard );
   }
}


void UIPage::updateUISections()
{
   for( SamplerGUI::UISection *pSection : m_UISections )
   {
      pSection->samplesUpdated();
   }
}


PluginEditor *UIPage::editor() const
{
   return( m_pEditor );
}
