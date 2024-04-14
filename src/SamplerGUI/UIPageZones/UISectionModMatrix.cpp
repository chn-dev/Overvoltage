#include "UISectionModMatrix.h"

#include <util.h>

using namespace SamplerGUI;

UISectionModMatrix::UIModSlot::UISlider::UISlider( const String &componentName, UIModSlot *pModSlot ) :
   juce::Slider( componentName ),
   m_pModSlot( pModSlot )
{
}


juce::String UISectionModMatrix::UIModSlot::UISlider::getTextFromValue( double value )
{
   SamplerEngine::ModMatrix::ModDestInfo modDestInfo = m_pModSlot->getModSlot()->getDest();
   return( stdvformat( modDestInfo.getFormat(), value ) + getTextValueSuffix().toStdString() );
}


UISectionModMatrix::UIModSlot::UIModSlot() :
   m_pSectionModMatrix( nullptr ),
   m_pModSlot( nullptr ),
   m_pcbSrc( nullptr ),
   m_pcbDest( nullptr ),
   m_pbEnabled( nullptr ),
   m_psAmount( nullptr ),
   m_Index( -1 ),
   m_Xp( -1 ),
   m_Yp( -1 )
{
}


UISectionModMatrix::UIModSlot::UIModSlot( UISectionModMatrix *pSectionModMatrix, SamplerEngine::ModMatrix::ModSlot *pModSlot, int index, int xp, int yp ) :
   m_pSectionModMatrix( pSectionModMatrix ),
   m_pModSlot( pModSlot ),
   m_Index( index ),
   m_Xp( xp ),
   m_Yp( yp )
{
   m_pbEnabled = new juce::TextButton( stdformat( "{}", index ) );
   m_pbEnabled->setToggleable( true );
   m_pbEnabled->setClickingTogglesState( true );
   m_pbEnabled->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pbEnabled->setToggleState( pModSlot->isEnabled(), false );
   m_pbEnabled->addListener( this );
   m_pbEnabled->setBounds( xp, yp, 18, 18 );
   pSectionModMatrix->addAndMakeVisible( m_pbEnabled );

   m_pcbSrc = new juce::ComboBox( "Source" );
   for( SamplerEngine::ModMatrix::ModSrc modSrc : SamplerEngine::ModMatrix::allModSrc() )
   {
      m_pcbSrc->addItem( SamplerEngine::ModMatrix::toString( modSrc ), modSrc );
   }
   m_pcbSrc->setSelectedId( pModSlot->getSrc() );
   m_pcbSrc->addListener( this );
   m_pcbSrc->setBounds( xp + 28, yp, 120, 18 );
   m_pcbSrc->setEnabled( m_pModSlot->isEnabled() );
   pSectionModMatrix->addAndMakeVisible( m_pcbSrc );

   m_psAmount = new UIModSlot::UISlider( "Amount", this );
   SamplerEngine::ModMatrix::ModDestInfo modDestInfo = m_pModSlot->getDest();
   m_psAmount->setRange( modDestInfo.getMin(), modDestInfo.getMax(), modDestInfo.getStep() );
   m_psAmount->setTextValueSuffix( modDestInfo.getUnit() );
   m_psAmount->setSliderStyle( juce::Slider::LinearHorizontal );
   m_psAmount->setTextBoxStyle( juce::Slider::TextBoxLeft, true, 48, 9 );
   m_psAmount->setSliderSnapsToMousePosition( false );
   m_psAmount->setLookAndFeel( &m_SliderLAF );
   m_psAmount->setValue( m_pModSlot->getAmount() );
   m_psAmount->addListener( this );
   m_psAmount->setColour( juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::fromRGBA( 0, 0, 0, 0 ) );
   m_psAmount->setBounds( xp + 28 + 120 + 4, yp + 4, 96, 12 );
   m_psAmount->setEnabled( m_pModSlot->isEnabled() && pModSlot->getDest() != SamplerEngine::ModMatrix::ModDest_None );
   pSectionModMatrix->addAndMakeVisible( m_psAmount );


   m_pcbDest = new juce::ComboBox( "Destination" );
   for( SamplerEngine::ModMatrix::ModDest modDest : SamplerEngine::ModMatrix::allModDest() )
   {
      m_pcbDest->addItem( SamplerEngine::ModMatrix::toString( modDest ), modDest );
   }
   m_pcbDest->setSelectedId( pModSlot->getDest() );
   m_pcbDest->addListener( this );
   m_pcbDest->setBounds( xp + 250, yp, 120, 18 );
   m_pcbDest->setEnabled( m_pModSlot->isEnabled() );
   pSectionModMatrix->addAndMakeVisible( m_pcbDest );
}


UISectionModMatrix::UIModSlot::~UIModSlot()
{
   if( m_pbEnabled )
   {
      delete m_pbEnabled;
   }

   if( m_psAmount )
   {
      delete m_psAmount;
   }

   if( m_pcbSrc )
   {
      delete m_pcbSrc;
   }

   if( m_pcbDest )
   {
      delete m_pcbDest;
   }
}


SamplerEngine::ModMatrix::ModSlot *UISectionModMatrix::UIModSlot::getModSlot() const
{
   return( m_pModSlot );
}


void UISectionModMatrix::UIModSlot::buttonClicked( Button *pButton )
{
   if( m_pbEnabled == pButton )
   {
      m_pModSlot->setEnabled( m_pbEnabled->getToggleState() );
      m_pcbSrc->setEnabled( m_pModSlot->isEnabled() );
      m_pcbDest->setEnabled( m_pModSlot->isEnabled() );
      m_psAmount->setEnabled( m_pModSlot->isEnabled() );
   }
}


void UISectionModMatrix::UIModSlot::buttonStateChanged( Button *pButton )
{
}


void UISectionModMatrix::UIModSlot::sliderValueChanged( Slider *pSlider )
{
   if( m_psAmount == pSlider )
   {
      m_pModSlot->setAmount( m_psAmount->getValue() );
   }
}


void UISectionModMatrix::UIModSlot::comboBoxChanged( ComboBox *pComboBox )
{
   if( pComboBox == m_pcbSrc )
   {
      SamplerEngine::ModMatrix::ModSrc selectedModSrc = (SamplerEngine::ModMatrix::ModSrc)m_pcbSrc->getSelectedId();
      if( selectedModSrc != m_pModSlot->getSrc() )
      {
         m_pModSlot->setSrc( selectedModSrc );
      }
   } else
   if( pComboBox == m_pcbDest )
   {
      SamplerEngine::ModMatrix::ModDest selectedModDest = (SamplerEngine::ModMatrix::ModDest)m_pcbDest->getSelectedId();
      if( selectedModDest != m_pModSlot->getDest() )
      {
         SamplerEngine::ModMatrix::ModDestInfo modDestInfo = selectedModDest;
         m_pModSlot->setDest( selectedModDest );
         m_pModSlot->setAmount( modDestInfo.getDefaultValue() );
         m_psAmount->setRange( modDestInfo.getMin(), modDestInfo.getMax(), modDestInfo.getStep() );
         m_psAmount->setTextValueSuffix( modDestInfo.getUnit() );
         m_psAmount->setValue( m_pModSlot->getAmount() );
      }
   }
}


UISectionModMatrix::UISectionModMatrix( UIPage *pUIPage ) :
   UISection( pUIPage, "Modulation Matrix" )
{
}


UISectionModMatrix::~UISectionModMatrix()
{
   for( int i = 0; i < m_UIModSlots.size(); i++ )
   {
      delete m_UIModSlots[i];
   }
   m_UIModSlots.clear();
}


void UISectionModMatrix::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 32, 64, 64 ) );
   g.fillAll();
}


void UISectionModMatrix::resized()
{
   UISection::resized();
}


void UISectionModMatrix::samplesUpdated()
{
   const SamplerEngine::Sample *pSample = sample();

   for( int i = 0; i < m_UIModSlots.size(); i++ )
   {
      delete m_UIModSlots[i];
   }
   m_UIModSlots.clear();

   if( pSample )
   {
      for( size_t i = 0; i < pSample->getModMatrix()->numSlots(); i++ )
      {
         UIModSlot *pUIModSlot = new UIModSlot( this, pSample->getModMatrix()->getSlot( i ), i, 6, 28 + ( 18 * i ) );
         m_UIModSlots.push_back( pUIModSlot );
      }
   }
}
