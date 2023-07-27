#pragma once

#include "UISection.h"
#include "PluginEditor.h"

UISection::UISection( AudioPluginAudioProcessorEditor *pEditor, std::string label ) :
   m_pSample( 0 ),
   m_pEditor( pEditor ),
   m_pLabel( 0 )
{
   m_pEditor->addUISection( this );

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
   m_pEditor->removeUISection( this );

   if( m_pLabel )
   {
      delete m_pLabel;
   }
}


AudioPluginAudioProcessorEditor *UISection::editor() const
{
   return( m_pEditor );
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


void UISection::CycleComponent::setCurrentItem( int item )
{
   setText( m_Items.at( item ), juce::dontSendNotification );
   m_CurrentItem = item;
}


int UISection::CycleComponent::getCurrentItem() const
{
   return( m_CurrentItem );
}


void UISection::CycleComponent::mouseDrag( const MouseEvent &event )
{
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
   if( item >= m_Items.size() )
      item = m_Items.size() - 1;

   if( getCurrentItem() != item )
   {
      setCurrentItem( item );
      callChangeListeners();
   }

}


void UISection::CycleComponent::mouseDown( const MouseEvent &event )
{
   m_MouseStartPoint = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition();
   m_OrigMouseCursor = getMouseCursor();
   setMouseCursor( juce::MouseCursor::NoCursor );
   m_OrigColour = findColour( ColourIds::outlineColourId );
   setColour( ColourIds::outlineColourId, juce::Colour::fromRGB( 255, 64, 64 ) );
   m_DY = 0;
}


void UISection::CycleComponent::mouseUp( const MouseEvent &event )
{
   setColour( ColourIds::outlineColourId, m_OrigColour );
   setMouseCursor( m_OrigMouseCursor );
}


void UISection::SliderLookAndFeel::drawLinearSlider(
   Graphics &g,
   int x, int y, int width, int height,
   float sliderPos, float minSliderPos, float maxSliderPos,
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

      float p = ( slider.getValue() - slider.getMinimum() ) / ( slider.getMaximum() - slider.getMinimum() );
      int xKnob = x2 + ( p * ( width - knobWidth ) );
      int yKnob = y2;

      // The transparent triangle
      g.setColour( juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
      Path tr;
      tr.addTriangle( x2, y2 - 1,
                      x3, y1,
                      x3, y2 - 1 );
      g.fillPath( tr );

      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      g.drawHorizontalLine( y2, x1, x4 );
      g.drawVerticalLine( x2, y1, y3 );
      g.drawVerticalLine( x3, y1, y3 );

      // Draw the knob
      g.setColour( juce::Colour::fromRGB( 196, 196, 196 ) );
      g.fillRoundedRectangle( xKnob - ( knobWidth / 2 ),
                              yKnob - ( knobHeight / 2 ),
                              knobWidth,
                              knobHeight,
                              6 );

      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawRoundedRectangle( xKnob - ( knobWidth / 2 ),
                              yKnob - ( knobHeight / 2 ),
                              knobWidth,
                              knobHeight,
                              6,
                              1.5 );
      // Line in the middle of the knob
      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawVerticalLine( xKnob, yKnob - ( knobHeight / 2 ) + 5,
                                 yKnob - ( knobHeight / 2 ) + knobHeight - 5 );
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

      float p = ( slider.getValue() - slider.getMinimum() ) / ( slider.getMaximum() - slider.getMinimum() );
      int xKnob = x2;
      int yKnob = y3 - ( p * ( height - knobHeight ) );

      // The transparent triangle
      g.setColour( juce::Colour::fromRGBA( 255, 255, 255, 64 ) );
      Path tr;
      tr.addTriangle( x2 + 1, y2,
                      x2 + 1, y3,
                      x3, y2 );
      g.fillPath( tr );

      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      g.drawVerticalLine( x2, y1, y4 );
      g.drawHorizontalLine( y2, x1, x3 );
      g.drawHorizontalLine( y3, x1, x3 );

      // Draw the knob
      g.setColour( juce::Colour::fromRGB( 196, 196, 196 ) );
      g.fillRoundedRectangle( xKnob - ( knobWidth / 2 ),
                              yKnob - ( knobHeight / 2 ),
                              knobWidth,
                              knobHeight,
                              6 );

      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawRoundedRectangle( xKnob - ( knobWidth / 2 ),
                              yKnob - ( knobHeight / 2 ),
                              knobWidth,
                              knobHeight,
                              6,
                              1.5 );

      // Line in the middle of the knob
      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
      g.drawHorizontalLine( yKnob, xKnob - ( knobWidth / 2 ) + 5,
                                   xKnob - ( knobWidth / 2 ) + knobWidth - 5 );
   }
}


void UISection::paint( juce::Graphics &g )
{
}


void UISection::resized()
{
   if( m_pLabel )
   {
      m_pLabel->setBounds( 0, 0, getBounds().getWidth(), 18 );
   }
}


void UISection::onSampleSelectionUpdated( SamplerKeyboard *pSamplerKeyboard )
{
   std::set<Sample *> samples = pSamplerKeyboard->selectedSamples();
   Sample *pSample;
   if( samples.size() != 1 )
   {
      pSample = 0;
   } else
   {
      pSample = *samples.begin();
   }
   
//   if( m_pSample != pSample )
   {
      m_pSample = pSample;
      sampleUpdated();
   }
}


Sample *UISection::sample() const
{
   return( m_pSample );
}
