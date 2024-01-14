#include "UIPageZones.h"
#include "PluginEditor.h"

using namespace SamplerGUI;

UIPageZones::UIPageZones( PluginEditor *pEditor ) :
   UIPage( pEditor )
{
   setBounds( 0, 32, 1024, 640 - 32 );
   int yofs = 0;

   m_pKeyboard = new SamplerGUI::SamplerKeyboard( editor() );
   addKeyListener( m_pKeyboard );
   m_pKeyboard->setBounds( 0, yofs, 64 + 128, 640 - yofs );
   m_pKeyboard->addListener( &( editor()->processor() ) );
   m_pKeyboard->addSamplerKeyboardListener( &( editor()->processor() ) );
   m_pKeyboard->setWidth( 64 );
   editor()->processor().juce::MidiKeyboardState::addListener( m_pKeyboard );
   addAndMakeVisible( m_pKeyboard );

   m_pWaveView = new SamplerGUI::WaveView( this );
   m_pWaveView->setBounds( 64 + 128 + 4, yofs, 1024 - ( 64 + 128 + 4 ), 196 );
   addAndMakeVisible( m_pWaveView );

   m_pSampleSection = new SamplerGUI::SampleUISection( this );
   m_pSampleSection->setBounds( 64 + 128 + 4, 196 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pSampleSection );

   m_pNameRangesUISection = new SamplerGUI::NameRangesUISection( this );
   m_pNameRangesUISection->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pNameRangesUISection );

   m_pAEGUISection = new SamplerGUI::AEGUISection( this );
   m_pAEGUISection->setBounds( 64 + 128 + 4, 196 + 4 + 128 + 4 + 128 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pAEGUISection );

   m_pOutputUISection = new SamplerGUI::OutputUISection( this );
   m_pOutputUISection->setBounds( 1024 - 64 - 128 - 4, 196 + 4 + yofs, 192, 128 );
   addAndMakeVisible( m_pOutputUISection );

   updateUISections();
}


UIPageZones::~UIPageZones()
{
   delete m_pKeyboard;
   delete m_pWaveView;
   delete m_pSampleSection;
   delete m_pNameRangesUISection;
   delete m_pAEGUISection;
   delete m_pOutputUISection;
}


void UIPageZones::currentPartChanged( size_t /*nPart*/ )
{
   m_pKeyboard->clearSelectedSamples();
}
