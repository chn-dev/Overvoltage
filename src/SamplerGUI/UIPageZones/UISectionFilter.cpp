#include "PluginEditor.h"
#include "UISectionFilter.h"

using namespace SamplerGUI;

UISectionFilter::UISectionFilter( UIPage *pUIPage ) :
   UISection( pUIPage, "Filter" )
{
   m_psCutoff = new juce::Slider( "Cutoff" );
   m_psCutoff->setRange( 0.0, 1.0, 0.001 );
   m_psCutoff->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psCutoff->setSliderSnapsToMousePosition( false );
   m_psCutoff->setLookAndFeel( &m_SliderLAF );
   m_psCutoff->addListener( this );
   m_psCutoff->setTextBoxStyle( juce::Slider::NoTextBox, true, 1, 1 );
   addAndMakeVisible( m_psCutoff );
   m_plCutoff = new juce::Label( juce::String(), "Cutoff:" );
   addAndMakeVisible( m_plCutoff );

   m_psResonance = new juce::Slider( "Resonance" );
   m_psResonance->setRange( 0.0, 1.0, 0.001 );
   m_psResonance->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psResonance->setSliderSnapsToMousePosition( false );
   m_psResonance->setLookAndFeel( &m_SliderLAF );
   m_psResonance->addListener( this );
   m_psResonance->setTextBoxStyle( juce::Slider::NoTextBox, true, 1, 1 );
   addAndMakeVisible( m_psResonance );
   m_plResonance = new juce::Label( juce::String(), "Resonance:" );
   addAndMakeVisible( m_plResonance );

}


UISectionFilter::~UISectionFilter()
{
   delete m_psCutoff;
   delete m_plCutoff;
   delete m_psResonance;
   delete m_plResonance;
}


void UISectionFilter::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void UISectionFilter::resized()
{
   UISection::resized();

   m_psCutoff->setBounds( 48, 32, 150, 20 );
   m_plCutoff->setBounds( 4, 32, 48, 10 );

   m_psResonance->setBounds( 48, 64, 150, 20 );
   m_plResonance->setBounds( 4, 64, 48, 10 );
}


void UISectionFilter::filterUpdated( SamplerEngine::Filter *pFilter )
{
   m_psCutoff->setValue( pFilter->getCutoff(), dontSendNotification );
   m_psResonance->setValue( pFilter->getResonance(), dontSendNotification );
}


void UISectionFilter::samplesUpdated()
{
   const SamplerEngine::Sample *pSample = sample();
   if( pSample )
   {
      filterUpdated( pSample->getFilter() );
   }

   m_plCutoff->setVisible( pSample != nullptr );
   m_psCutoff->setVisible( pSample != nullptr );
   m_plResonance->setVisible( pSample != nullptr );
   m_psResonance->setVisible( pSample != nullptr );
}


void UISectionFilter::sliderValueChanged( Slider *pSlider )
{
   if( samples().size() <= 0 )
      return;

   if( m_psCutoff == pSlider )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         pSample->getFilter()->setCutoff( m_psCutoff->getValue() );
      }
   } else
   if( m_psResonance == pSlider )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         pSample->getFilter()->setResonance( m_psResonance->getValue() );
      }
   }
}
