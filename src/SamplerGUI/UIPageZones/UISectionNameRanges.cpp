#include "PluginEditor.h"
#include "UISectionNameRanges.h"

#include "util.h"

using namespace SamplerGUI;

UISectionNameRanges::UISectionNameRanges( UIPage *pUIPage ) :
   UISection( pUIPage, "Name & Ranges" )
{
   std::vector<std::string> noteNames;
   for( int i = 0; i < 128; i++ )
   {
      noteNames.push_back( UISectionKeyboard::noteNameWithOctave( i ).toStdString() );
   }

   m_plLow = new juce::Label( juce::String(), "low" );
   addAndMakeVisible( m_plLow );
   m_plHigh = new juce::Label( juce::String(), "hi" );
   addAndMakeVisible( m_plHigh );
   m_plBaseNote = new juce::Label( juce::String(), "root" );
   addAndMakeVisible( m_plBaseNote );
   m_plK1 = new juce::Label( juce::String(), "K" );
   addAndMakeVisible( m_plK1 );
   m_plK2 = new juce::Label( juce::String(), "K" );
   addAndMakeVisible( m_plK2 );
   m_plV1 = new juce::Label( juce::String(), "V" );
   addAndMakeVisible( m_plV1 );
   m_plV2 = new juce::Label( juce::String(), "V" );
   addAndMakeVisible( m_plV2 );

   m_plKeytrack = new juce::Label( juce::String(), "Keytrack:" );
   addAndMakeVisible( m_plKeytrack );

   m_pcKeytrack = new CycleComponent();
   m_pcKeytrack->setItems( -400.0, 400.0, 1.0, "{:.0f}", "%" );
   m_pcKeytrack->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcKeytrack->addListener( this );
   m_pcKeytrack->setJustificationType( juce::Justification::centred );
   addAndMakeVisible( m_pcKeytrack );

   m_pcMinNote = new CycleComponent( noteNames );
   m_pcMinNote->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMinNote->addListener( this );
   m_pcMinNote->setJustificationType( juce::Justification::centred );
   addAndMakeVisible( m_pcMinNote );

   m_pcBaseNote = new CycleComponent( noteNames );
   m_pcBaseNote->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcBaseNote->addListener( this );
   m_pcBaseNote->setJustificationType( juce::Justification::centred );
   addAndMakeVisible( m_pcBaseNote );

   m_pcMaxNote = new CycleComponent( noteNames );
   m_pcMaxNote->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMaxNote->addListener( this );
   m_pcMaxNote->setJustificationType( juce::Justification::centred );
   addAndMakeVisible( m_pcMaxNote );

   std::vector<std::string> velNames;
   for( int i = 0; i < 128; i++ )
   {
      velNames.push_back( stdformat( "{}", i ) );
   }
   m_pcMinVelocity = new CycleComponent( velNames );
   m_pcMinVelocity->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMinVelocity->addListener( this );
   m_pcMinVelocity->setJustificationType( juce::Justification::centred );
   addAndMakeVisible( m_pcMinVelocity );

   m_pcMaxVelocity = new CycleComponent( velNames );
   m_pcMaxVelocity->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pcMaxVelocity->addListener( this );
   m_pcMaxVelocity->setJustificationType( juce::Justification::centred );
   addAndMakeVisible( m_pcMaxVelocity );

   m_plName = new juce::Label( juce::String(), "" );
   m_plName->addListener( this );
   m_plName->setJustificationType( juce::Justification::left );
   m_plName->setEditable( true );
   m_plName->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_plName->setColour( juce::Label::ColourIds::outlineWhenEditingColourId, juce::Colour::fromRGBA( 255, 255, 255, 128 ) );
   m_plName->addListener( this );
   addAndMakeVisible( m_plName );
}


UISectionNameRanges::~UISectionNameRanges()
{
   delete m_pcMinNote;
   delete m_pcMaxNote;
   delete m_pcMinVelocity;
   delete m_pcMaxVelocity;
   delete m_plLow;
   delete m_plHigh;
   delete m_plK1;
   delete m_plK2;
   delete m_plV1;
   delete m_plV2;
   delete m_pcKeytrack;
   delete m_plKeytrack;
   delete m_plBaseNote;
   delete m_pcBaseNote;
   delete m_plName;
}


void UISectionNameRanges::labelTextChanged( Label *pLabel )
{
   if( samples().size() != 1 )
      return;

   if( pLabel == m_plName )
   {
      sample()->setName( m_plName->getText().toStdString() );
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMinVelocity )
   {
      sample()->setMinVelocity( m_pcMinVelocity->getCurrentItem() );
      if( sample()->getMaxVelocity() < sample()->getMinVelocity() )
      {
         sample()->setMaxVelocity( sample()->getMinVelocity() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMaxVelocity )
   {
      sample()->setMaxVelocity( m_pcMaxVelocity->getCurrentItem() );
      if( sample()->getMinVelocity() > sample()->getMaxVelocity() )
      {
         sample()->setMinVelocity( sample()->getMaxVelocity() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMinNote )
   {
      sample()->setMinNote( m_pcMinNote->getCurrentItem() );
      if( sample()->getMaxNote() < sample()->getMinNote() )
      {
         sample()->setMaxNote( sample()->getMinNote() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcMaxNote )
   {
      sample()->setMaxNote( m_pcMaxNote->getCurrentItem() );
      if( sample()->getMinNote() > sample()->getMaxNote() )
      {
         sample()->setMinNote( sample()->getMaxNote() );
      }
      uiPage()->editor()->repaint();
   } else
   if( pLabel == m_pcBaseNote )
   {
      sample()->setBaseNote( m_pcBaseNote->getCurrentItem() );
   } else
   if( pLabel == m_pcKeytrack )
   {
      sample()->setKeytrack( m_pcKeytrack->getCurrentItem() - 400.0 );
   }
   uiPage()->editor()->repaint();

   samplesUpdated();
}


void UISectionNameRanges::paint( juce::Graphics &g )
{
   UISection::paint( g );

   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void UISectionNameRanges::resized()
{
   UISection::resized();
   int d = 20;

   m_plName->setBounds( 8, 28, getBounds().getWidth() - 16, 18 );

   m_plK1->setBounds( 0, 48 + d, 18, 14 );
   m_plV1->setBounds( 0, 68 + d, 18, 14 );
   m_plLow->setBounds( 32, 32 + d, 32, 14 );
   m_plBaseNote->setBounds( 80, 32 + d, 32, 14 );
   m_plHigh->setBounds( 128, 32 + d, 32, 14 );
   m_plK2->setBounds( getBounds().getWidth() - 18, 48 + d, 18, 14 );
   m_plV2->setBounds( getBounds().getWidth() - 18, 68 + d, 18, 14 );
   m_pcMinNote->setBounds( 32, 48 + d, 38, 14 );
   m_pcBaseNote->setBounds( 80, 48 + d, 38, 14 );
   m_pcMaxNote->setBounds( 128, 48 + d, 38, 14 );
   m_pcMinVelocity->setBounds( 32, 68 + d, 38, 14 );
   m_pcMaxVelocity->setBounds( 128, 68 + d, 38, 14 );
   m_plKeytrack->setBounds( 0, 88 + d, 56, 14 );
   m_pcKeytrack->setBounds( 60, 88 + d, 76, 14 );
}


void UISectionNameRanges::samplesUpdated()
{
   bool ok = samples().size() == 1;

   m_pcMinNote->setVisible( ok );
   m_pcMaxNote->setVisible( ok );
   m_pcMinVelocity->setVisible( ok );
   m_pcMaxVelocity->setVisible( ok );
   m_plLow->setVisible( ok );
   m_plHigh->setVisible( ok );
   m_plK1->setVisible( ok );
   m_plK2->setVisible( ok );
   m_plV1->setVisible( ok );
   m_plV2->setVisible( ok );
   m_plBaseNote->setVisible( ok );
   m_pcBaseNote->setVisible( ok );
   m_plName->setVisible( ok );
   m_plKeytrack->setVisible( ok );
   m_pcKeytrack->setVisible( ok );

   if( ok )
   {
      m_pcMinNote->setCurrentItem( sample()->getMinNote() );
      m_pcMaxNote->setCurrentItem( sample()->getMaxNote() );
      m_pcBaseNote->setCurrentItem( sample()->getBaseNote() );
      m_pcMinVelocity->setCurrentItem( sample()->getMinVelocity() );
      m_pcMaxVelocity->setCurrentItem( sample()->getMaxVelocity() );
      m_plName->setText( sample()->getName(), juce::dontSendNotification );
      m_pcKeytrack->setCurrentItem( (int)( sample()->getKeytrack() + 400.5 ) );
   }
}
