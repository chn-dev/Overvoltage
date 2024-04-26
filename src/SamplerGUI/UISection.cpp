#include <util.h>

#include "UISection.h"
#include "UIPage.h"
#include "PluginEditor.h"

using namespace SamplerGUI;

UISection::UISection( UIPage *pUIPage, std::string label ) :
   m_pUIPage( pUIPage ),
   m_pLabel( nullptr )
{
   m_pUIPage->addUISection( this );

   if( label != std::string() )
   {
      m_pLabel = new juce::Label( label, label );
      m_pLabel->setJustificationType( juce::Justification::centred );
      m_pLabel->setColour( juce::Label::ColourIds::backgroundColourId, juce::Colour::fromRGB( 64, 64, 128 ) );
      m_pLabel->setColour( juce::Label::ColourIds::outlineColourId, juce::Colour::fromRGB( 64, 64, 255 ) );
      juce::Font font = m_pLabel->getFont();
      font.setSizeAndStyle( 16.0, font.getDefaultStyle(), font.getHorizontalScale(), 0.0 );
      m_pLabel->setFont( font );
      addAndMakeVisible( m_pLabel );
   }
}


UISection::~UISection()
{
   m_pUIPage->removeUISection( this );

   if( m_pLabel )
   {
      delete m_pLabel;
   }
}


UIPage *UISection::uiPage() const
{
   return( m_pUIPage );
}


UISection::CycleComponent::CycleComponent() :
   m_PixelsPerItem( 16 ),
   m_OrigMouseCursor( juce::MouseCursor::NormalCursor )
{
   setCurrentItem( -1 );
}


UISection::CycleComponent::CycleComponent( std::vector<std::string> items ) :
   m_Items( items ),
   m_PixelsPerItem( 16 ),
   m_OrigMouseCursor( juce::MouseCursor::NormalCursor )
{
   setCurrentItem( 0 );
}


UISection::CycleComponent::~CycleComponent()
{
}


void UISection::CycleComponent::setItems( double minVal, double maxVal, double step, std::string format, std::string unit )
{
   m_Items.clear();

   for( int i = 0; ; i++ )
   {
      double value = minVal + ( step * i );
      if( value > maxVal )
         break;
      m_Items.push_back( stdvformat( format, value ) + unit );
   }

   setCurrentItem( 0 );
}


void UISection::CycleComponent::setCurrentItem( int item )
{
   if( item < 0 || item >= (int)m_Items.size() )
   {
      setText( "", juce::dontSendNotification );
      m_CurrentItem = -1;
   } else
   {
      setText( m_Items.at( (size_t)item ), juce::dontSendNotification );
      m_CurrentItem = item;
   }
}


int UISection::CycleComponent::getCurrentItem() const
{
   return( m_CurrentItem );
}


void UISection::CycleComponent::mouseDrag( const MouseEvent &event )
{
   if( !isEnabled() )
      return;

   juce::Point<float> newMousePosition = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition();
   if( newMousePosition == m_MouseStartPoint )
      return;

   m_DY += event.getDistanceFromDragStartY();
   juce::Desktop::getInstance().getMainMouseSource().setScreenPosition( m_MouseStartPoint );
   if( ( m_DY / m_PixelsPerItem ) == 0 )
      return;

   int dItem = -m_DY / m_PixelsPerItem;
   m_DY += dItem * m_PixelsPerItem;
   int item = dItem + getCurrentItem();
   if ( item < 0 )
      item = 0;
   else
   if( (size_t)item >= m_Items.size() )
      item = (int)m_Items.size() - 1;

   if( getCurrentItem() != item )
   {
      setCurrentItem( item );
      callChangeListeners();
   }

}


void UISection::CycleComponent::mouseDown( const MouseEvent &/*event*/ )
{
   if( !isEnabled() )
      return;

   m_MouseStartPoint = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition();
   m_OrigMouseCursor = getMouseCursor();
   setMouseCursor( juce::MouseCursor::NoCursor );
   m_OrigColour = findColour( ColourIds::outlineColourId );
   setColour( ColourIds::outlineColourId, juce::Colour::fromRGB( 255, 64, 64 ) );
   m_DY = 0;
}


void UISection::CycleComponent::mouseUp( const MouseEvent &/*event*/ )
{
   if( !isEnabled() )
      return;

   setColour( ColourIds::outlineColourId, m_OrigColour );
   setMouseCursor( m_OrigMouseCursor );
}


void UISection::SliderLookAndFeel::drawLinearSlider(
   Graphics &g,
   int x, int y, int width, int height,
   float /*sliderPos*/, float /*minSliderPos*/, float /*maxSliderPos*/,
   const Slider::SliderStyle sliderStyle, Slider &slider )
{
   if( sliderStyle == Slider::SliderStyle::LinearHorizontal )
   {
      int knobWidth = width / 6;
      int knobHeight = height;

      int x1 = x;
      int x2 = x + ( knobWidth / 2 );
      int x3 = x + width - ( knobWidth / 2 );
      int x4 = x + width - 1;
      int y1 = y + ( height / 2 ) - ( knobHeight / 2 );
      int y2 = y + ( height / 2 ) - 1;
      int y3 = y + ( height / 2 ) + ( knobHeight / 2 );

      float p = (float)( ( slider.getValue() - slider.getMinimum() ) / ( slider.getMaximum() - slider.getMinimum() ) );
      int xKnob = (int)( x2 + ( p * ( width - knobWidth ) ) );
      int yKnob = y2;

      // The transparent triangle
      g.setColour( juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
      Path tr;
      tr.addTriangle( (float)x2, (float)( y2 - 1 ),
                      (float)x3, (float)y1,
                      (float)x3, (float)( y2 - 1 ) );
      g.fillPath( tr );

      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      g.drawHorizontalLine( y2, (float)x1, (float)x4 );
      g.drawVerticalLine( x2, (float)y1, (float)y3 );
      g.drawVerticalLine( x3, (float)y1, (float)y3 );

      // Draw the knob
      g.setColour( juce::Colour::fromRGB( 196, 196, 196 ) );
      g.fillRoundedRectangle( (float)( xKnob - ( knobWidth / 2 ) ),
                              (float)( yKnob - ( knobHeight / 2 ) ),
                              (float)knobWidth,
                              (float)knobHeight,
                              6 );

      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawRoundedRectangle( (float)( xKnob - ( knobWidth / 2 ) ),
                              (float)( yKnob - ( knobHeight / 2 ) ),
                              (float)knobWidth,
                              (float)knobHeight,
                              6,
                              1.5 );
      // Line in the middle of the knob
      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawVerticalLine( xKnob, (float)( yKnob - ( knobHeight / 2 ) + 5 ),
                                 (float)( yKnob - ( knobHeight / 2 ) + knobHeight - 5 ) );
   } else
   if( sliderStyle == Slider::SliderStyle::LinearVertical )
   {
      int knobWidth = width;
      int knobHeight = height / 6;

      int y1 = y;
      int y2 = y + ( knobHeight / 2 );
      int y3 = y + height - ( knobHeight / 2 );
      int y4 = y + height - 1;
      int x1 = x + ( width / 2 ) - ( knobWidth / 2 );
      int x2 = x + ( width / 2 ) - 1;
      int x3 = x + ( width / 2 ) + ( knobWidth / 2 );

      float p = (float)( ( slider.getValue() - slider.getMinimum() ) / ( slider.getMaximum() - slider.getMinimum() ) );
      int xKnob = x2;
      int yKnob = (int)( y3 - ( p * ( height - knobHeight ) ) );

      // The transparent triangle
      g.setColour( juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
      Path tr;
      tr.addTriangle( (float)( x2 + 1 ), (float)y2,
                      (float)( x2 + 1 ), (float)y3,
                      (float)x3, (float)y2 );
      g.fillPath( tr );

      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      g.drawVerticalLine( x2, (float)y1, (float)y4 );
      g.drawHorizontalLine( y2, (float)x1, (float)x3 );
      g.drawHorizontalLine( y3, (float)x1, (float)x3 );

      // Draw the knob
      g.setColour( juce::Colour::fromRGB( 196, 196, 196 ) );
      g.fillRoundedRectangle( (float)( xKnob - ( knobWidth / 2 ) ),
                              (float)( yKnob - ( knobHeight / 2 ) ),
                              (float)knobWidth,
                              (float)knobHeight,
                              6 );

      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawRoundedRectangle( (float)( xKnob - ( knobWidth / 2 ) ),
                              (float)( yKnob - ( knobHeight / 2 ) ),
                              (float)knobWidth,
                              (float)knobHeight,
                              6,
                              1.5 );

      // Line in the middle of the knob
      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawHorizontalLine( yKnob, (float)( xKnob - ( knobWidth / 2 ) + 5 ),
                                   (float)( xKnob - ( knobWidth / 2 ) + knobWidth - 5 ) );
   }
}


void UISection::paint( juce::Graphics &/*g*/ )
{
}


void UISection::resized()
{
   if( m_pLabel )
   {
      m_pLabel->setBounds( 0, 0, getBounds().getWidth(), 18 );
   }
}


void UISection::onSampleSelectionUpdated( UISectionSamplerKeyboard *pSamplerKeyboard )
{
   m_Samples = pSamplerKeyboard->selectedSamples();
   samplesUpdated();
}


const std::set<SamplerEngine::Sample *> &UISection::samples() const
{
   return( m_Samples );
}

SamplerEngine::Sample *UISection::sample() const
{
   if( m_Samples.size() <= 0 )
      return( nullptr );
   else
      return( *m_Samples.begin() );
}
