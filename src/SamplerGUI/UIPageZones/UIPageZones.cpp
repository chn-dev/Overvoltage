#include "UIPageZones.h"
#include "PluginEditor.h"
#include "UISectionSamplerKeyboard.h"

using namespace SamplerGUI;

UIPageZones::UIPageZones( PluginEditor *pEditor ) :
   UIPage( pEditor )
{
   setBounds( 0, 32, 1024, 640 - 32 );
   int yofs = 0;

   m_pUISectionKeyboard = new SamplerGUI::UISectionSamplerKeyboard( this );
   addKeyListener( m_pUISectionKeyboard );
   m_pUISectionKeyboard->setBounds( 0, yofs, 64 + 128, 640 - yofs );
   m_pUISectionKeyboard->addListener( &( editor()->processor() ) );
   m_pUISectionKeyboard->addSamplerKeyboardListener( &( editor()->processor() ) );
   m_pUISectionKeyboard->setWidth( 64 );
   editor()->processor().juce::MidiKeyboardState::addListener( m_pUISectionKeyboard );
   addAndMakeVisible( m_pUISectionKeyboard );

   m_pUISectionWaveView = new SamplerGUI::UISectionWaveView( this );
   m_pUISectionWaveView->setBounds( 64 + 128 + 4, yofs, 1024 - ( 64 + 128 + 4 ), 196 );
   addAndMakeVisible( m_pUISectionWaveView );

   m_pUISectionSample = new SamplerGUI::UISectionSample( this );
   m_pUISectionSample->setBounds( 64 + 128 + 4, 196 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pUISectionSample );

   m_pUISectionNameRanges = new SamplerGUI::UISectionNameRanges( this );
   m_pUISectionNameRanges->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pUISectionNameRanges );

   m_pUISectionAEG = new SamplerGUI::UISectionAEG( this );
   m_pUISectionAEG->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + 128 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pUISectionAEG );

   m_pUISectionOutput = new SamplerGUI::UISectionOutput( this );
   m_pUISectionOutput->setBounds( 1024 - 64 - 128 - 4, 196 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pUISectionOutput );

   updateUISections();
}


UIPageZones::~UIPageZones()
{
   delete m_pUISectionKeyboard;
   delete m_pUISectionWaveView;
   delete m_pUISectionSample;
   delete m_pUISectionNameRanges;
   delete m_pUISectionAEG;
   delete m_pUISectionOutput;
}


void UIPageZones::currentPartChanged( size_t /*nPart*/ )
{
   m_pUISectionKeyboard->clearSelectedSamples();
}
