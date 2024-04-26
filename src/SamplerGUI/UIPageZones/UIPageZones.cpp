#include "UIPageZones.h"
#include "PluginEditor.h"
#include "UISectionSamplerKeyboard.h"

using namespace SamplerGUI;

UIPageZones::UIPageZones( PluginEditor *pEditor ) :
   UIPage( pEditor )
{
   setBounds( 0, 32, 1024, 640 - 32 );
   int yofs = 0;

   int kbWidth = 64 + 128;
   int xStart = kbWidth;
   int yStart = 196;
   int editorSectionWidth = 1024 - kbWidth;
   int editorSectionHeight = 640 - 32 - yStart;
   int margin = 2;

   m_pUISectionKeyboard = new SamplerGUI::UISectionSamplerKeyboard( this );
   pEditor->addKeyListener( m_pUISectionKeyboard );
   m_pUISectionKeyboard->setBounds( 0, yofs, kbWidth, 640 - yofs );
   m_pUISectionKeyboard->addListener( &( editor()->processor() ) );
   m_pUISectionKeyboard->addSamplerKeyboardListener( &( editor()->processor() ) );
   m_pUISectionKeyboard->setWidth( 64 );
   editor()->processor().juce::MidiKeyboardState::addListener( m_pUISectionKeyboard );
   addAndMakeVisible( m_pUISectionKeyboard );

   m_pUISectionWaveView = new SamplerGUI::UISectionWaveView( this );
   m_pUISectionWaveView->setBounds( kbWidth + 4, yofs, 1024 - ( 64 + 128 + 4 ), yStart );
   addAndMakeVisible( m_pUISectionWaveView );


   // First row
   m_pUISectionSample = new SamplerGUI::UISectionSample( this );
   m_pUISectionSample->setBounds(
      xStart + margin,
      yStart + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionSample );

   m_pUISectionFilter = new SamplerGUI::UISectionFilter( this );
   m_pUISectionFilter->setBounds(
      xStart + ( editorSectionWidth / 4 ) + margin,
      yStart + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionFilter );

   m_pUISectionOutput = new SamplerGUI::UISectionOutput( this );
   m_pUISectionOutput->setBounds(
      xStart + ( ( 3 * editorSectionWidth ) / 4 ) + margin,
      yStart + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionOutput );

   // Second row
   m_pUISectionNameRanges = new SamplerGUI::UISectionNameRanges( this );
   m_pUISectionNameRanges->setBounds(
      xStart + margin,
      yStart + ( editorSectionHeight / 3 ) + margin,
      ( editorSectionWidth / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionNameRanges );

   m_pUISectionLFO = new SamplerGUI::UISectionLFO( this, "LFO" );
   m_pUISectionLFO->setBounds(
      xStart + ( ( editorSectionWidth / 4 ) * 1 ) + margin,
      yStart + ( editorSectionHeight / 3 ) + margin,
      ( ( editorSectionWidth * 2 ) / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionLFO );

   // Third row
   m_pUISectionAEG = new SamplerGUI::UISectionAEG( this );
   m_pUISectionAEG->setBounds(
      xStart + margin,
      yStart + ( ( 2 * editorSectionHeight ) / 3 ) + margin,
      ( editorSectionWidth / 8 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionAEG );

   m_pUISectionEG2 = new SamplerGUI::UISectionEG2( this );
   m_pUISectionEG2->setBounds(
      xStart + ( ( editorSectionWidth / 8 ) * 1 ) + margin,
      yStart + ( ( 2 * editorSectionHeight ) / 3 ) + margin,
      ( editorSectionWidth / 8 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionEG2 );

   m_pUISectionModMatrix = new SamplerGUI::UISectionModMatrix( this );
   m_pUISectionModMatrix->setBounds(
      xStart + ( ( editorSectionWidth / 4 ) * 1 ) + margin,
      yStart + ( ( 2 * editorSectionHeight ) / 3 ) + margin,
      ( ( editorSectionWidth * 3 ) / 4 ) - ( margin * 2 ),
      ( editorSectionHeight / 3 ) - ( margin * 2 ) );
   addAndMakeVisible( m_pUISectionModMatrix );

   updateUISections();
}


UIPageZones::~UIPageZones()
{
   delete m_pUISectionKeyboard;
   delete m_pUISectionWaveView;
   delete m_pUISectionSample;
   delete m_pUISectionNameRanges;
   delete m_pUISectionLFO;
   delete m_pUISectionAEG;
   delete m_pUISectionEG2;
   delete m_pUISectionFilter;
   delete m_pUISectionOutput;
   delete m_pUISectionModMatrix;
}


void UIPageZones::currentPartChanged( size_t /*nPart*/ )
{
   m_pUISectionKeyboard->clearSelectedSamples();
}
