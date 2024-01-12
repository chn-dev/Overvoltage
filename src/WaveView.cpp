#include "PluginEditor.h"
#include "WaveView.h"

#include "util.h"

WaveView::WaveView( AudioPluginAudioProcessorEditor *pEditor ) :
   UISection( pEditor ),
   m_IsDraggingLoopStart( false ),
   m_IsDraggingLoopEnd( false ),
   m_IsSelecting( false ),
   m_SelectionStart( ~(uint32_t)0 ),
   m_SelectionEnd( ~(uint32_t)0 ),
   m_SampleViewStart( ~(uint32_t)0 ),
   m_SampleViewEnd( ~(uint32_t)0 )
{
   m_pbZoom = new juce::TextButton( "Z" );
   m_pbZoom->addListener( this );
   addAndMakeVisible( m_pbZoom );

   m_pbShowAll = new juce::TextButton( "A" );
   m_pbShowAll->addListener( this );
   addAndMakeVisible( m_pbShowAll );

   m_pbZoomIn = new juce::TextButton( "+" );
   m_pbZoomIn->addListener( this );
   addAndMakeVisible( m_pbZoomIn );

   m_pbZoomOut = new juce::TextButton( "-" );
   m_pbZoomOut->addListener( this );
   addAndMakeVisible( m_pbZoomOut );

   m_psScrollBar = new juce::ScrollBar( false );
   m_psScrollBar->addListener( this );
   addAndMakeVisible( m_psScrollBar );
}


WaveView::~WaveView()
{
   delete m_pbZoom;
   delete m_pbShowAll;
   delete m_pbZoomOut;
   delete m_pbZoomIn;
   delete m_psScrollBar;
}


void WaveView::paint( juce::Graphics &g )
{
   UISection::paint( g );

   int totalHeight = getBounds().getHeight() - 14 - 8;

   if( samples().size() != 1 )
   {
      g.setColour( juce::Colour::fromRGB( 255, 0, 0 ) );
      g.drawLine( 0, 0, (float)getWidth(), (float)totalHeight );
      g.drawLine( (float)getBounds().getWidth(), 0, 0, (float)totalHeight );
   } else
   {
      const WaveFile *pWave = sample()->getWave();

      if( ( m_SelectionStart != ~(decltype( m_SelectionStart ))0 ) && ( m_SelectionEnd != ~(decltype( m_SelectionEnd ))0 ) )
      {
         uint32_t s = m_SelectionStart;
         uint32_t e = m_SelectionEnd;
         if( s > e )
         {
            uint32_t tmp = s;
            s = e;
            e = tmp;
         }

         int x1 = getXPosFromSampleNum( s );
         int x2 = getXPosFromSampleNum( e );
         g.setColour( juce::Colour::fromRGB( 96, 0, 0 ) );
         g.fillRect( x1, 0, x2 - x1, totalHeight );
      }

      for( int nChannel = 0; nChannel < pWave->numChannels(); nChannel++ )
      {
         int yTop = ( nChannel * totalHeight ) / pWave->numChannels();
         int yBottom = ( ( nChannel + 1 ) * totalHeight ) / pWave->numChannels();
         int yCenter = ( yTop + yBottom ) / 2;

         g.setColour( juce::Colour::fromRGB( 255, 0, 0 ) );
         g.drawLine( 0, (float)yCenter, (float)( getBounds().getWidth() - 1 ), (float)yCenter );

         g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
         int prevY = -1;

         for( int j = 0; j < getBounds().getWidth() - 2; j++ )
         {
            uint32_t nSample = getSampleNumFromXPos( j );
            float v = pWave->floatValue( nChannel, nSample );
            if( v != NAN )
            {
               int y = (int)( ( v * (float)totalHeight ) / (float)pWave->numChannels() );
               y /= 2;

               if( j > 0 )
               {
                  g.drawLine( (float)( j - 1 + 1 ), (float)( prevY + yCenter ), (float)( j + 1 ), (float)( y + yCenter ) );
               }

               prevY = y;
            }
         }
      }

      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      g.drawText( stdformat( "{}", sample()->getName()),
         8, totalHeight + 8, 128, 14, juce::Justification::left );

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
         192, totalHeight + 8, 128, 14, juce::Justification::left );
      g.drawText( stdformat( "{:.3f}s", (float)pWave->numSamples() / (float)pWave->sampleRate() ),
         320, totalHeight + 8, 128, 14, juce::Justification::left );

      g.drawText( stdformat( "{:.1f}kHz", (float)pWave->sampleRate() / 1000.0 ),
         500, totalHeight + 8, 128, 14, juce::Justification::left );
      g.drawText( stdformat( "{}bit {}ch", pWave->numBits(), pWave->numChannels() ),
         560, totalHeight + 8, 128, 14, juce::Justification::left );

      g.setColour( juce::Colour::fromRGB( 255, 255, 0 ) );

      Path tr;
      if( pWave->loopStart() >= getSampleViewStart() &&
          pWave->loopStart() <= getSampleViewEnd() )
      {
         int loopStartX = getXPosFromSampleNum( pWave->loopStart() );
         g.drawVerticalLine( loopStartX, 0, (float)totalHeight );
         tr.addTriangle( (float)loopStartX, 0,
                         (float)( loopStartX + 12 ), 0,
                         (float)loopStartX, 12 );
      }

      if( pWave->loopEnd() >= getSampleViewStart() &&
          pWave->loopEnd() <= getSampleViewEnd() )
      {
         int loopEndX = getXPosFromSampleNum( pWave->loopEnd() );
         g.drawVerticalLine( loopEndX, 0, (float)totalHeight );
         tr.addTriangle( (float)loopEndX, 0,
                         (float)( loopEndX - 12 ), 0,
                         (float)loopEndX, 12 );
      }

      g.fillPath( tr );

      g.setColour( juce::Colour::fromRGB( 128, 128, 128 ) );
      g.drawRect( 0, totalHeight - 1, getBounds().getWidth(), 10 );
      g.drawRect( 0, totalHeight + 8, getBounds().getWidth(), 14 );
   }

   g.setColour( juce::Colour::fromRGB( 128, 128, 128 ) );
   g.drawRect( 0, 0, getBounds().getWidth(), totalHeight );
}


void WaveView::resized()
{
   UISection::resized();

   m_pbShowAll->setBounds(
      getBounds().getWidth() - ( 1 * 14 ),
      getBounds().getHeight() - 14,
      14,
      14 );

   m_pbZoom->setBounds(
      getBounds().getWidth() - ( 2 * 14 ),
      getBounds().getHeight() - 14,
      14,
      14 );

   m_pbZoomOut->setBounds(
      getBounds().getWidth() - ( 3 * 14 ),
      getBounds().getHeight() - 14,
      14,
      14 );

   m_pbZoomIn->setBounds(
      getBounds().getWidth() - ( 4 * 14 ),
      getBounds().getHeight() - 14,
      14,
      14 );

   m_psScrollBar->setBounds(
      0,
      getBounds().getHeight() - 14 - 8,
      getBounds().getWidth(),
      8 );
}


void WaveView::samplesUpdated()
{
   m_IsSelecting = false;
   m_SelectionStart = ~(decltype( m_SelectionStart ))0;
   m_SelectionEnd = ~(decltype( m_SelectionEnd ))0;
   m_SampleViewStart = ~(decltype( m_SampleViewStart ))0;
   m_SampleViewEnd = ~(decltype( m_SampleViewEnd ))0;

   m_pbZoom->setVisible( samples().size() == 1 );
   m_pbShowAll->setVisible( samples().size() == 1 );
   m_pbZoomIn->setVisible( samples().size() == 1 );
   m_pbZoomOut->setVisible( samples().size() == 1 );
   m_psScrollBar->setVisible( samples().size() == 1 );

   if( samples().size() == 1 )
   {
      m_psScrollBar->setRangeLimits( 0.0, sample()->getWave()->numSamples() );
      m_psScrollBar->setCurrentRange( getSampleViewStart(), getSampleViewEnd() - getSampleViewStart() );
   }

   repaint();
}


void WaveView::mouseMove( const MouseEvent &event )
{
   m_IsDraggingLoopStart = false;
   m_IsDraggingLoopEnd = false;

   if( !sample() )
      return;

   const WaveFile *pWave = sample()->getWave();
   int loopStartX = getXPosFromSampleNum( pWave->loopStart() );
   int loopEndX = getXPosFromSampleNum( pWave->loopEnd() );

   int xp = event.getPosition().getX();
   int yp = event.getPosition().getY();
   if( xp >= loopStartX - 4 && xp < loopStartX + 4 && yp >= 0 && yp < getBounds().getHeight() - 14 - 8 )
   {
      setMouseCursor( juce::MouseCursor::LeftEdgeResizeCursor );
      m_IsDraggingLoopStart = true;
      m_IsDraggingLoopEnd = false;
   } else
   if( xp >= loopEndX - 4 && xp < loopEndX + 4 && yp >= 0 && yp < getBounds().getHeight() - 14 - 8 )
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
   } else
   {
      if( event.getPosition().getY() <= getBounds().getHeight() - 14 )
      {
         m_IsSelecting = true;
         m_SelectionStart = getSampleNumFromXPos( event.getPosition().getX() );
         m_SelectionEnd = m_SelectionStart;
      }
   }
}


uint32_t WaveView::getSampleNumFromXPos( int xPos ) const
{
   uint32_t numSamples = getSampleViewEnd() - getSampleViewStart() + 1;
   float fDx = (float)xPos / ( (float)getBounds().getWidth() - 2 );
   uint32_t nSample = (uint32_t)( fDx * ( numSamples - 1 ) );
   nSample = nSample + getSampleViewStart();
   return( nSample );
}


int WaveView::getXPosFromSampleNum( uint32_t sampleNum ) const
{
   uint32_t numSamples = getSampleViewEnd() - getSampleViewStart() + 1;
   sampleNum = sampleNum - getSampleViewStart();
   return( ( (int)sampleNum * ( getBounds().getWidth() - 2 ) / (int)numSamples ) + 1 );
}


uint32_t WaveView::getSampleViewStart() const
{
   if( ( m_SampleViewStart == ~(decltype( m_SampleViewStart ))0 ) || ( m_SampleViewEnd == ~(decltype( m_SampleViewEnd ))0 ) )
   {
      if( sample() )
      {
         return( 0 );
      }
   }

   return( m_SampleViewStart );
}


uint32_t WaveView::getSampleViewEnd() const
{
   if( ( m_SampleViewStart == ~(decltype( m_SampleViewStart ))0 ) || ( m_SampleViewEnd == ~(decltype( m_SampleViewEnd ))0 ) )
   {
      if( sample() )
      {
         return( sample()->getWave()->numSamples() - 1 );
      }
   }

   return( m_SampleViewEnd );
}



void WaveView::mouseDrag( const MouseEvent &event )
{
   if( !sample() )
      return;

   if( !m_IsDraggingLoopStart && !m_IsDraggingLoopEnd && !m_IsSelecting )
      return;

   WaveFile *pWave = sample()->getWave();

   if( m_IsSelecting )
   {
      m_SelectionEnd = getSampleNumFromXPos( event.getPosition().getX() );
   } else
   if( m_IsDraggingLoopStart || m_IsDraggingLoopEnd )
   {
      uint32_t newLoopPoint = getSampleNumFromXPos( event.getPosition().getX() );

      if( m_IsDraggingLoopStart )
      {
         pWave->setLoopStart( newLoopPoint );
      } else
      if( m_IsDraggingLoopEnd )
      {
         pWave->setLoopEnd( newLoopPoint );
      }

      if( pWave->loopStart() > pWave->loopEnd() )
      {
         uint32_t tmp = pWave->loopStart();
         pWave->setLoopStart( pWave->loopEnd() );
         pWave->setLoopEnd( tmp );
      }
   }

   repaint();
}


void WaveView::mouseUp( const MouseEvent &/*event*/ )
{
   m_IsSelecting = false;
   if( m_SelectionStart == m_SelectionEnd )
   {
      m_SelectionStart = ~(decltype( m_SelectionStart ))0;
      m_SelectionEnd = ~(decltype( m_SelectionEnd ))0;
   }

   repaint();
}


void WaveView::buttonClicked( Button *pButton )
{
   if( pButton == m_pbZoomIn )
   {
      uint32_t vStart = getSampleViewStart();
      uint32_t vEnd = getSampleViewEnd();
      if( vEnd - vStart >= 16 )
      {
         uint32_t len = vEnd - vStart;
         vStart += len / 4;
         vEnd -= len / 4;
         m_SampleViewStart = vStart;
         m_SampleViewEnd = vEnd;
         m_psScrollBar->setCurrentRange( getSampleViewStart(), getSampleViewEnd() - getSampleViewStart() );
         repaint();
      }
   } else
   if( pButton == m_pbZoomOut )
   {
      uint32_t vStart = getSampleViewStart();
      uint32_t vEnd = getSampleViewEnd();
      uint32_t len = vEnd - vStart;
      uint32_t nl = len / 2;
      if( nl > vStart )
         vStart = 0;
      else
         vStart -= nl;

      vEnd += nl;
      if( vEnd >= sample()->getWave()->numSamples() )
         vEnd = sample()->getWave()->numSamples() - 1;
      m_SampleViewStart = vStart;
      m_SampleViewEnd = vEnd;
      m_psScrollBar->setCurrentRange( getSampleViewStart(), getSampleViewEnd() - getSampleViewStart() );
      repaint();
   } else
   if( pButton == m_pbZoom )
   {
      if( ( m_SelectionStart != ~(decltype( m_SelectionStart ))0 ) && ( m_SelectionEnd != ~(decltype( m_SelectionEnd ))0 ) )
      {
         uint32_t selStart = m_SelectionStart < m_SelectionEnd ? m_SelectionStart : m_SelectionEnd;
         uint32_t selEnd =   m_SelectionStart < m_SelectionEnd ? m_SelectionEnd : m_SelectionStart;
         m_SampleViewStart = selStart;
         m_SampleViewEnd = selEnd;
         m_SelectionStart = ~(decltype( m_SelectionStart ))0;
         m_SelectionEnd = ~(decltype( m_SelectionEnd ))0;

         m_psScrollBar->setCurrentRange( getSampleViewStart(), getSampleViewEnd() - getSampleViewStart() );

         repaint();
      }
   } else
   if( pButton == m_pbShowAll )
   {
      m_SampleViewStart = ~(decltype( m_SampleViewStart ))0;
      m_SampleViewEnd = ~(decltype( m_SampleViewEnd ))0;
      m_psScrollBar->setCurrentRange( getSampleViewStart(), getSampleViewEnd() );
      repaint();
   }
}


void WaveView::buttonStateChanged( Button */*pButton*/ )
{
}


void WaveView::scrollBarMoved( ScrollBar *pScrollBar, double /*newRangeStart*/ )
{
   if( pScrollBar == m_psScrollBar )
   {
      double rStart = m_psScrollBar->getCurrentRangeStart();
      double rSize = m_psScrollBar->getCurrentRangeSize();
      m_SampleViewStart = (uint32_t)rStart;
      m_SampleViewEnd = (uint32_t)( rStart + rSize );
      repaint();
   }
}
