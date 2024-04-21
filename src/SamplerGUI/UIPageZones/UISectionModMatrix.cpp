#include "UISectionModMatrix.h"

#include <util.h>

using namespace SamplerGUI;

UISectionModMatrix::UIModSlot::UIModSlot() :
   m_pSectionModMatrix( nullptr ),
   m_pcbSrc( nullptr ),
   m_pcbDest( nullptr ),
   m_pbEnabled( nullptr ),
   m_pccAmount( nullptr ),
   m_pModSlot( nullptr ),
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
   m_pbEnabled->setToggleState( pModSlot->isEnabled(), dontSendNotification );
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

   m_pccAmount = new CycleComponent();
   m_pccAmount->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
   m_pccAmount->addListener( this );
   m_pccAmount->setJustificationType( juce::Justification::centred );
   m_pccAmount->setBounds( xp + 28 + 120 + 4, yp, 94, 18 );
   SamplerEngine::ModMatrix::ModDestInfo modDestInfo = pModSlot->getDest();
   m_pccAmount->setItems(
      modDestInfo.getMin(),
      modDestInfo.getMax(),
      modDestInfo.getStep(),
      modDestInfo.getFormat(),
      modDestInfo.getUnit() );
   int nAmount = (int)( ( ( pModSlot->getAmount() - modDestInfo.getMin() ) / modDestInfo.getStep() ) + 0.5 );
   m_pccAmount->setCurrentItem( nAmount );
   m_pccAmount->setEnabled( m_pModSlot->isEnabled() );
   pSectionModMatrix->addAndMakeVisible( m_pccAmount );

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

   if( m_pccAmount )
   {
      delete m_pccAmount;
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
      m_pccAmount->setEnabled( m_pModSlot->isEnabled() );
   }
}


void UISectionModMatrix::UIModSlot::buttonStateChanged( Button */*pButton*/ )
{
}

void UISectionModMatrix::UIModSlot::labelTextChanged( Label *pLabel )
{
   if( m_pccAmount == pLabel )
   {
      SamplerEngine::ModMatrix::ModDestInfo modDestInfo = m_pModSlot->getDest();
      m_pModSlot->setAmount( ( m_pccAmount->getCurrentItem() * modDestInfo.getStep() ) + modDestInfo.getMin() );
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
         m_pccAmount->setItems(
            modDestInfo.getMin(),
            modDestInfo.getMax(),
            modDestInfo.getStep(),
            modDestInfo.getFormat(),
            modDestInfo.getUnit() );
         int nAmount = (int)( ( ( m_pModSlot->getAmount() - modDestInfo.getMin() ) / modDestInfo.getStep() ) + 0.5 );
         m_pccAmount->setCurrentItem( nAmount );
      }
   }
}


UISectionModMatrix::UISectionModMatrix( UIPage *pUIPage ) :
   UISection( pUIPage, "Modulation Matrix" )
{
}


UISectionModMatrix::~UISectionModMatrix()
{
   for( size_t i = 0; i < m_UIModSlots.size(); i++ )
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

   for( size_t i = 0; i < m_UIModSlots.size(); i++ )
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
