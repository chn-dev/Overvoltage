#ifdef __GNUC__
#include <fmt/core.h>
#define stdformat fmt::format
#else
#include <format>
#define stdformat std::format
#endif

#include "PluginEditor.h"
#include "WaveView.h"

WaveView::WaveView( AudioPluginAudioProcessorEditor *pEditor ) :
   UISection( pEditor ),
   m_IsDraggingLoopStart( false ),
   m_IsDraggingLoopEnd( false )
{
}


WaveView::~WaveView()
{
}


void WaveView::paint( juce::Graphics &g )
{
   UISection::paint( g );

   int totalHeight = getBounds().getHeight() - 14;

   if( !sample() )
   {
      g.setColour( juce::Colour::fromRGB( 255, 0, 0 ) );
      g.drawLine( 0, 0, getWidth(), totalHeight );
      g.drawLine( getBounds().getWidth(), 0, 0, totalHeight );
   } else
   {
      const WaveFile *pWave = sample()->getWave();

      for( int nChannel = 0; nChannel < pWave->numChannels(); nChannel++ )
      {
         int yTop = ( nChannel * totalHeight ) / pWave->numChannels();
         int yBottom = ( ( nChannel + 1 ) * totalHeight ) / pWave->numChannels();
         int yCenter = ( yTop + yBottom ) / 2;

         g.setColour( juce::Colour::fromRGB( 255, 0, 0 ) );
         g.drawLine( 0, yCenter, getBounds().getWidth() - 1, yCenter );

         g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
         int prevY = -1;
         for( int j = 0; j < getBounds().getWidth() - 2; j++ )
         {
            float fj = (float)j / ( (float)getBounds().getWidth() - 2 );
            int nSample = (int)( fj * (float)pWave->numSamples() );
            float v = pWave->floatValue( nChannel, nSample );
            if( v != NAN )
            {
               int y = ( v * (float)totalHeight ) / (float)pWave->numChannels();
               y /= 2;

               if( j > 0 )
               {
                  g.drawLine( j - 1 + 1, prevY + yCenter, j + 1, y + yCenter );
               }

               prevY = y;
            }
         }
      }

      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      g.drawText( stdformat( "{}", sample()->getName()), 8, totalHeight, 128, 14, juce::Justification::left );

      std::string sizeString;
      if( pWave->size() < 1024 ) // < 1KB
      {
         sizeString = stdformat( "{} Bytes", pWave->size() );
      } else
      if( pWave->size() < 1024 * 1024 )
      {
         sizeString = stdformat( "{:.3f} KB", (float)pWave->size() / 1024.0 );
      } else
      {
         sizeString = stdformat( "{:.3f} MB", (float)pWave->size() / 1024.0 / 1024.0 );
      }

      g.drawText( stdformat( "{}", sizeString ),
         192, totalHeight, 128, 14, juce::Justification::left );
      g.drawText( stdformat( "{:.3f}s", (float)pWave->numSamples() / (float)pWave->sampleRate() ),
         320, totalHeight, 128, 14, juce::Justification::left );

      g.drawText( stdformat( "{:.1f}kHz", (float)pWave->sampleRate() / 1000.0 ),
         500, totalHeight, 128, 14, juce::Justification::left );
      g.drawText( stdformat( "{}bit {}ch", pWave->numBits(), pWave->numChannels() ),
         560, totalHeight, 128, 14, juce::Justification::left );

      g.setColour( juce::Colour::fromRGB( 255, 255, 0 ) );
      int loopStartX = ( pWave->loopStart() * ( getBounds().getWidth() - 2 ) / pWave->numSamples() ) + 1;
      int loopEndX = ( pWave->loopEnd() * ( getBounds().getWidth() - 2 ) / pWave->numSamples() ) + 1;
      g.drawVerticalLine( loopStartX, 0, totalHeight );
      g.drawVerticalLine( loopEndX, 0, totalHeight );

      Path tr;
      tr.addTriangle( loopStartX, 0,
                      loopStartX + 12, 0,
                      loopStartX, 12 );
      tr.addTriangle( loopEndX, 0,
                      loopEndX - 12, 0,
                      loopEndX, 12 );
      g.fillPath( tr );
   }

   g.setColour( juce::Colour::fromRGB( 128, 128, 128 ) );
   g.drawRect( 0, 0, getBounds().getWidth(), totalHeight );
}


void WaveView::resized()
{
   UISection::resized();
}

void WaveView::sampleUpdated()
{
   repaint();
}


void WaveView::mouseMove( const MouseEvent &event )
{
   m_IsDraggingLoopStart = false;
   m_IsDraggingLoopEnd = false;

   if( !sample() )
      return;

   const WaveFile *pWave = sample()->getWave();
   int loopStartX = ( pWave->loopStart() * ( getBounds().getWidth() - 2 ) / pWave->numSamples() ) + 1;
   int loopEndX = ( pWave->loopEnd() * ( getBounds().getWidth() - 2 ) / pWave->numSamples() ) + 1;

   int xp = event.getPosition().getX();
   int yp = event.getPosition().getY();
   if( xp >= loopStartX - 4 && xp < loopStartX + 4 /* && yp >= 0 && yp < 12*/ )
   {
      setMouseCursor( juce::MouseCursor::LeftEdgeResizeCursor );
      m_IsDraggingLoopStart = true;
      m_IsDraggingLoopEnd = false;
   } else
   if( xp >= loopEndX - 4 && xp < loopEndX + 4 /* && yp >= 0 && yp < 12*/ )
   {
      setMouseCursor( juce::MouseCursor::RightEdgeResizeCursor );
      m_IsDraggingLoopStart = false;
      m_IsDraggingLoopEnd = true;
   } else
   {
      setMouseCursor( juce::MouseCursor::NormalCursor );
   }
}


void WaveView::mouseDown( const MouseEvent &event )
{
   if( !sample() )
      return;

   if( m_IsDraggingLoopStart )
   {
      m_OrigLoopPoint = sample()->getWave()->loopStart();
   } else
   if( m_IsDraggingLoopEnd )
   {
      m_OrigLoopPoint = sample()->getWave()->loopEnd();
   }
}


void WaveView::mouseDrag( const MouseEvent &event )
{
   if( !sample() )
      return;

   if( !m_IsDraggingLoopStart && !m_IsDraggingLoopEnd )
      return;

   WaveFile *pWave = sample()->getWave();

   int dx = event.getDistanceFromDragStartX();
   float fDx = (float)dx / ( (float)getBounds().getWidth() - 2 );
   int mSamples = fDx * ( sample()->getWave()->numSamples() - 1 );
   
   if( m_IsDraggingLoopStart )
   {
      pWave->setLoopStart( m_OrigLoopPoint + mSamples );
   } else
   if( m_IsDraggingLoopEnd )
   {
      pWave->setLoopEnd( m_OrigLoopPoint + mSamples );
   }

   if( pWave->loopStart() > pWave->loopEnd() )
   {
      uint32_t tmp = pWave->loopStart();
      pWave->setLoopStart( pWave->loopEnd() );
      pWave->setLoopEnd( tmp );
   }

   repaint();
}


void WaveView::mouseUp( const MouseEvent &event )
{

}
