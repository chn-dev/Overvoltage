#include "PluginEditor.h"
#include "EGUISection.h"


EGUISection::EGUISection( PluginEditor *pEditor, std::string label ) :
   UISection( pEditor, label )
{
   m_psAttack = new juce::Slider( "Attack" );
   m_psAttack->setRange( 0.0, 1.0, 0.001 );
   m_psAttack->setSliderStyle( juce::Slider::LinearVertical );
   m_psAttack->setSliderSnapsToMousePosition( false );
   m_psAttack->setLookAndFeel( &m_SliderLAF );
   m_psAttack->addListener( this );
   addAndMakeVisible( m_psAttack );
   m_plAttack = new juce::Label( juce::String(), "A" );
   addAndMakeVisible( m_plAttack );

   m_psDecay = new juce::Slider( "Decay" );
   m_psDecay->setRange( 0.0, 1.0, 0.001 );
   m_psDecay->setSliderStyle( juce::Slider::LinearVertical );
   m_psDecay->setSliderSnapsToMousePosition( false );
   m_psDecay->setLookAndFeel( &m_SliderLAF );
   m_psDecay->addListener( this );
   addAndMakeVisible( m_psDecay );
   m_plDecay = new juce::Label( juce::String(), "D" );
   addAndMakeVisible( m_plDecay );

   m_psSustain = new juce::Slider( "Sustain" );
   m_psSustain->setRange( 0.0, 1.0, 0.001 );
   m_psSustain->setSliderStyle( juce::Slider::LinearVertical );
   m_psSustain->setSliderSnapsToMousePosition( false );
   m_psSustain->setLookAndFeel( &m_SliderLAF );
   m_psSustain->addListener( this );
   addAndMakeVisible( m_psSustain );
   m_plSustain = new juce::Label( juce::String(), "S" );
   addAndMakeVisible( m_plSustain );

   m_psRelease = new juce::Slider( "Release" );
   m_psRelease->setRange( 0.0, 1.0, 0.001 );
   m_psRelease->setSliderStyle( juce::Slider::LinearVertical );
   m_psRelease->setSliderSnapsToMousePosition( false );
   m_psRelease->setLookAndFeel( &m_SliderLAF );
   m_psRelease->addListener( this );
   addAndMakeVisible( m_psRelease );
   m_plRelease = new juce::Label( juce::String(), "R" );
   addAndMakeVisible( m_plRelease );
}


EGUISection::~EGUISection()
{
   delete m_psAttack;
   delete m_plAttack;
   delete m_psDecay;
   delete m_plDecay;
   delete m_psSustain;
   delete m_plSustain;
   delete m_psRelease;
   delete m_plRelease;
}


void EGUISection::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void EGUISection::resized()
{
   UISection::resized();

   m_psAttack->setBounds( 16, 16, 20, 100 );
   m_plAttack->setBounds( 17, 108, 18, 16 );

   m_psDecay->setBounds( 64, 16, 20, 100 );
   m_plDecay->setBounds( 65, 108, 18, 16 );

   m_psSustain->setBounds( 112, 16, 20, 100 );
   m_plSustain->setBounds( 113, 108, 18, 16 );

   m_psRelease->setBounds( 160, 16, 20, 100 );
   m_plRelease->setBounds( 161, 108, 18, 16 );
}


void EGUISection::egUpdated( ENV *pENV )
{
   if( !pENV )
      return;

   m_psAttack->setValue( pENV->getAttack(), dontSendNotification );
   m_psDecay->setValue( pENV->getDecay(), dontSendNotification );
   m_psSustain->setValue( pENV->getSustain(), dontSendNotification );
   m_psRelease->setValue( pENV->getRelease(), dontSendNotification );
}


void EGUISection::samplesUpdated()
{
   if( sample() )
   {
      egUpdated( getENV( sample() ) );
   }

   m_psAttack->setVisible( sample() != nullptr );
   m_plAttack->setVisible( sample() != nullptr );
   m_psDecay->setVisible( sample() != nullptr );
   m_plDecay->setVisible( sample() != nullptr );
   m_psSustain->setVisible( sample() != nullptr );
   m_plSustain->setVisible( sample() != nullptr );
   m_psRelease->setVisible( sample() != nullptr );
   m_plRelease->setVisible( sample() != nullptr );
}


void EGUISection::sliderValueChanged( Slider *pSlider )
{
   if( samples().size() <= 0 )
      return;

   if( pSlider == m_psAttack )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setAttack( m_psAttack->getValue() );
      }
   } else
   if( pSlider == m_psDecay )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setDecay( m_psDecay->getValue() );
      }
   } else
   if( pSlider == m_psSustain )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setSustain( m_psSustain->getValue() );
      }
   } else
   if( pSlider == m_psRelease )
   {
      for( SamplerEngine::Sample *pSample : samples() )
      {
         getENV( pSample )->setRelease( m_psRelease->getValue() );
      }
   }
}
