#include <filesystem>

#include <SamplerEngine/WaveFile.h>

#include "PluginEditor.h"
#include "UISectionSamplerKeyboard.h"

#include "util.h"

using namespace SamplerGUI;

UISectionSamplerKeyboard::UISectionSamplerKeyboard( UIPage *pPage ) :
   UISectionKeyboard( pPage ),
   m_pCurrentSample( nullptr ),
   m_CurrentSampleNote( -1 ),
   m_CurrentSampleNoteOffset( 0 ),
   m_CurrentSampleHandle( -1 ),
   m_DragDropNote( -1 ),
   m_SelectionStartPoint( juce::Point<int>( 0, 0 ) ),
   m_SelectionRectangle( juce::Rectangle<int>( 0, 0, 0, 0 ) ),
   m_Selecting( false )
{
}


UISectionSamplerKeyboard::~UISectionSamplerKeyboard()
{
}


void UISectionSamplerKeyboard::samplesUpdated()
{
}


void UISectionSamplerKeyboard::addSamplerKeyboardListener( UISectionSamplerKeyboardListener *pListener )
{
   if( pListener )
   {
      m_Listeners.push_back( pListener );
   }
}


bool UISectionSamplerKeyboard::keyPressed( const KeyPress &/*key*/ )
{
   return( false );
}


bool UISectionSamplerKeyboard::keyPressed( const KeyPress &key, Component */*pOriginatingComponent*/ )
{
   if( key == KeyPress::deleteKey )
   {
      for( SamplerEngine::Sample *pSample : m_SelectedSamples )
      {
         emitDeleteSample( m_pPage->editor()->currentPart(), pSample );
      }

      m_SelectedSamples.clear();
      emitSampleSelectionUpdated();
      repaint();
   }
   return( true );
}


bool UISectionSamplerKeyboard::keyStateChanged( bool /*isKeyDown*/ )
{
   return( false );
}


bool UISectionSamplerKeyboard::keyStateChanged( bool /*isKeyDown*/, Component */*pOriginatingComponent*/ )
{
   return( true );
}


SamplerEngine::Sample *UISectionSamplerKeyboard::getSampleAt( int x, int y ) const
{
   for( SamplerEngine::Sample *pSample : constSamples() )
   {
      juce::Rectangle<int> r = getNoteRect( pSample->getMinNote(), pSample->getMaxNote(), pSample->getMinVelocity(), pSample->getMaxVelocity() );
      if( r.contains( x, y ) )
         return( pSample );
   }

   return( nullptr );
}


bool UISectionSamplerKeyboard::drawSample( juce::Graphics &g, SamplerEngine::Sample *const pSample ) const
{
   bool highlighted = false;

   juce::Rectangle<int> r = getNoteRect( pSample );

   bool isSelected = m_SelectedSamples.find( pSample ) != m_SelectedSamples.end();

   if( m_pPage->editor()->processor().samplerEngine()->isPlaying( m_pPage->editor()->currentPart(), pSample ) )
   {
      g.setColour( juce::Colour::fromRGB( 255, 64, 64 ) );
   } else
   if( isSelected )
   {
      g.setColour( juce::Colour::fromRGB( 255, 128, 64 ) );
   } else
   {
      g.setColour( juce::Colour::fromRGB( 128, 128, 128 ) );
   }
   g.fillRect( r );

   if( ( r.contains( getMouseXYRelative() )  )
       && !isSelected
       && !m_Selecting )
   {
      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      highlighted = true;
   } else
   {
      g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
   }

   g.drawRect( r.getX(), r.getY(), r.getWidth(), r.getHeight() + 1 );

   g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
   g.drawText( pSample->getName(), r.getX(), r.getY() + 2, r.getWidth(), r.getHeight() - 4, juce::Justification::centred );

   return( highlighted );
}


void UISectionSamplerKeyboard::paint( juce::Graphics &g )
{
   UISectionKeyboard::paint( g );

   g.setFont( 13.0 );
   std::list<SamplerEngine::Sample *> highlighted;
   for( SamplerEngine::Sample *pSample : samples() )
   {
      if( drawSample( g, pSample ) )
      {
         highlighted.push_back( pSample );
      }
   }

   for( SamplerEngine::Sample *pSample : highlighted )
   {
      drawSample( g, pSample );
   }

   if( m_DragDropNote >= 0 )
   {
      g.setColour( juce::Colour::fromRGB( 255, 255, 255 ) );
      juce::Rectangle<int> r = getNoteRect( m_DragDropNote, m_DragDropNote, 0, 127 );
      g.drawRect( r );
      g.drawText( noteName( m_DragDropNote ), m_Width, r.getY(), getBounds().getWidth() - m_Width, m_KeyHeight / 2, juce::Justification::centred);
   }

   if( m_Selecting )
   {
      juce::Rectangle<int> r = m_SelectionRectangle;
      g.setColour( juce::Colour::fromRGBA( 128, 64, 64, 128 ) );
      g.fillRect( r );
      g.setColour( juce::Colour::fromRGB( 128, 64, 64 ) );
      g.drawRect( r );
   }
}


juce::Rectangle<int> UISectionSamplerKeyboard::getNoteRect( SamplerEngine::Sample *const pSample ) const
{
   int minNote = pSample->getMinNote();
   int maxNote = pSample->getMaxNote();
   int minVel = pSample->getMinVelocity();
   int maxVel = pSample->getMaxVelocity();

   if( m_SelectedSamples.find( pSample ) != m_SelectedSamples.end() &&
       m_CurrentSampleNote >= 0 )
   {
      minNote += m_CurrentSampleNoteOffset;
      maxNote += m_CurrentSampleNoteOffset;
   }

   return( getNoteRect( minNote, maxNote, minVel, maxVel ) );
}


juce::Rectangle<int> UISectionSamplerKeyboard::getNoteRect( int minNote, int maxNote, int minVel, int maxVel ) const
{
   int y = getBounds().getHeight() - ( ( ( maxNote - m_NoteOffset + 1 ) * m_KeyHeight ) / 2 );
   int maxW = getBounds().getWidth() - m_Width - 1;
   int w = ( maxW * ( maxVel - minVel ) ) / 127;
   int xp = m_Width + ( ( maxW * minVel ) / 127 );
   return( juce::Rectangle<int>( xp,
                                 y,
                                 w + 1,
                                 ( m_KeyHeight / 2 ) * ( maxNote - minNote + 1 ) ) );
}


bool UISectionSamplerKeyboard::isInterestedInFileDrag( const StringArray &files )
{
   for( String f : files )
   {
      if( !juce::String( util::toLower( f.toStdString() ) ).endsWith( ".wav" ) )
      {
         return( false );
      }
   }

   return( true );
}


int UISectionSamplerKeyboard::dragDropNote( int y ) const
{
   return( ( ( getBounds().getHeight() - y ) / ( m_KeyHeight / 2 ) ) + m_NoteOffset );
}


int UISectionSamplerKeyboard::dragDropVelocity( int x ) const
{
   x -= m_Width;
   int w = getBounds().getWidth() - m_Width;
   if( x < 0 )
      x = 0;
   else
   if( x > w )
      x = w;
   return( ( x * 127 ) / w );
}


void UISectionSamplerKeyboard::fileDragEnter( const StringArray &/*files*/, int /*x*/, int y )
{
   m_DragDropNote = dragDropNote( y );
   repaint();
}


void UISectionSamplerKeyboard::fileDragMove( const StringArray &/*files*/, int /*x*/, int y )
{
   m_DragDropNote = dragDropNote( y );
   repaint();
}


void UISectionSamplerKeyboard::fileDragExit( const StringArray &/*files*/ )
{
   m_DragDropNote = -1;
   repaint();
}


void UISectionSamplerKeyboard::filesDropped( const StringArray &files, int /*x*/, int /*y*/ )
{
   int n = 0;
   for( String f : files )
   {
      SamplerEngine::WaveFile *pWave = SamplerEngine::WaveFile::load( f.toStdString() );
      if( pWave )
      {
         SamplerEngine::Sample *pSample = new SamplerEngine::Sample( std::filesystem::path( f.toStdString() ).stem().string(), pWave, m_DragDropNote + n, m_DragDropNote + n );
         samples().push_back( pSample );
         n++;
      }
   }

   m_DragDropNote = -1;
   repaint();
}


void UISectionSamplerKeyboard::updateCursor( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   SamplerEngine::Sample *pSample = getSampleAt( x, y );
   if( pSample )
   {
      bool isSelected = m_SelectedSamples.find( pSample ) != m_SelectedSamples.end();
      size_t numSelected = m_SelectedSamples.size();

      juce::Rectangle<int> r = getNoteRect( pSample );

      bool top = false;
      bool bottom = false;
      bool left = false;
      bool right = false;

      if( isSelected )
      {
         if( numSelected == 1 )
         {
            top = y >= r.getY() && y < ( r.getY() + 3 );
            bottom = y >= ( r.getY() + r.getHeight() - 3 ) && y < ( r.getY() + r.getHeight() );
            left = x >= r.getX() && x < ( r.getX() + 1 );
            right = x >= ( r.getX() + r.getWidth() - 1 ) && x < ( r.getX() + r.getWidth() );
         }

         if( top )
         {
            setMouseCursor( juce::MouseCursor::TopEdgeResizeCursor );
            m_CurrentSampleHandle = 0;
         } else
         if( bottom )
         {
            setMouseCursor( juce::MouseCursor::BottomEdgeResizeCursor );
            m_CurrentSampleHandle = 2;
         } else
         if( left )
         {
            setMouseCursor( juce::MouseCursor::LeftEdgeResizeCursor );
            m_CurrentSampleHandle = 3;
         } else
         if( right )
         {
            setMouseCursor( juce::MouseCursor::RightEdgeResizeCursor );
            m_CurrentSampleHandle = 1;
         } else
         {
            setMouseCursor( juce::MouseCursor::DraggingHandCursor );
            m_CurrentSampleHandle = 256;
         }
      } else
      {
         setMouseCursor( juce::MouseCursor::NormalCursor );
         m_CurrentSampleHandle = -1;
      }
   } else
   {
      setMouseCursor( juce::MouseCursor::NormalCursor );
      m_CurrentSampleHandle = -1;
   }
}


void UISectionSamplerKeyboard::mouseMove( const MouseEvent &event)
{
   UISectionKeyboard::mouseMove( event );

   if( m_CurrentSampleNote >= 0 )
   {
   } else
   {
      updateCursor( event );
   }

   repaint();
}


void UISectionSamplerKeyboard::mouseDrag( const MouseEvent &event )
{
   UISectionKeyboard::mouseDrag( event );

   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   if( m_Selecting )
   {
      if( x < m_Width )
         x = m_Width;

      juce::Rectangle<int> r  = juce::Rectangle<int>(
         m_SelectionStartPoint.getX(),
         m_SelectionStartPoint.getY(),
         x - m_SelectionStartPoint.getX(),
         y - m_SelectionStartPoint.getY() );

      if( r.getWidth() < 0 )
      {
         r.setX( r.getX() + r.getWidth() );
         r.setWidth( -r.getWidth() );
      }

      if( r.getHeight() < 0 )
      {
         r.setY( r.getY() + r.getHeight() );
         r.setHeight( -r.getHeight() );
      }

      m_SelectionRectangle = r;

      m_SelectedSamples.clear();
      for( SamplerEngine::Sample *pSample : samples() )
      {
         juce::Rectangle<int> noteRect = getNoteRect( pSample );
         if( m_SelectionRectangle.contains( noteRect ) ||
             m_SelectionRectangle.intersects( noteRect ) )
         {
            m_SelectedSamples.insert( pSample );
         }
      }

      emitSampleSelectionUpdated();
      repaint();
   } else
   {
      int note = dragDropNote( event.getPosition().getY() );
      int velocity = dragDropVelocity( event.getPosition().getX() );
      int ofs = note - m_CurrentSampleNote;

      if( m_CurrentSampleHandle == 256 )
      {
         if( m_CurrentSampleNoteOffset != ofs )
         {
            m_CurrentSampleNoteOffset = ofs;
            repaint();
         }
      } else
      if( m_CurrentSampleHandle == 0 )
      {
         // top
         if( m_SelectedSamples.size() == 1 )
         {
            SamplerEngine::Sample *pSample = *m_SelectedSamples.begin();
            if( pSample->getMaxNote() != note )
            {
               pSample->setMaxNote( note );
               pSample->correctMinMaxNote();
               emitSampleSelectionUpdated();
               repaint();
            }
         }
      } else
      if( m_CurrentSampleHandle == 1 )
      {
         // right
         if( m_SelectedSamples.size() == 1 )
         {
            SamplerEngine::Sample *pSample = *m_SelectedSamples.begin();
            if( pSample->getMaxVelocity() != velocity )
            {
               pSample->setMaxVelocity( velocity );
               if( pSample->getMaxVelocity() < pSample->getMinVelocity() )
               {
                  pSample->setMaxVelocity( pSample->getMinVelocity() );
               }
               emitSampleSelectionUpdated();
               repaint();
            }
         }
      } else
      if( m_CurrentSampleHandle == 3 )
      {
         // left
         if( m_SelectedSamples.size() == 1 )
         {
            SamplerEngine::Sample *pSample = *m_SelectedSamples.begin();
            if( pSample->getMinVelocity() != velocity )
            {
               pSample->setMinVelocity( velocity );
               if( pSample->getMinVelocity() > pSample->getMaxVelocity() )
               {
                  pSample->setMinVelocity( pSample->getMaxVelocity() );
               }
               emitSampleSelectionUpdated();
               repaint();
            }
         }
      } else
      if( m_CurrentSampleHandle == 2 )
      {
         // bottom
         if( m_SelectedSamples.size() == 1 )
         {
            SamplerEngine::Sample *pSample = *m_SelectedSamples.begin();
            pSample->setMinNote( note );
            pSample->correctMinMaxNote();
            emitSampleSelectionUpdated();
            repaint();
         }
      }
   }
}


void UISectionSamplerKeyboard::mouseEnter( const MouseEvent &event )
{
   UISectionKeyboard::mouseEnter( event );
}


void UISectionSamplerKeyboard::mouseExit( const MouseEvent &event )
{
   UISectionKeyboard::mouseExit( event );
}


void UISectionSamplerKeyboard::mouseDown( const MouseEvent &event )
{
   UISectionKeyboard::mouseDown( event );

   int x = event.getMouseDownX();
   int y = event.getMouseDownY();

   SamplerEngine::Sample *pSample = getSampleAt( x, y );
   if( pSample )
   {
      bool isCtrlDown = juce::ModifierKeys::getCurrentModifiers().isCtrlDown();

      if( m_SelectedSamples.find( pSample ) == m_SelectedSamples.end() )
      {
         if( !isCtrlDown )
         {
            m_SelectedSamples.clear();
         }
         m_SelectedSamples.insert( pSample );
      } else
      {
         if( isCtrlDown )
         {
            m_SelectedSamples.erase( pSample );
         }
      }

      m_CurrentSampleNote = dragDropNote( event.getMouseDownY() );
      m_CurrentSampleNoteOffset = 0;

      emitSampleSelectionUpdated();
   } else
   if( x >= m_Width )
   {
      m_SelectedSamples.clear();

      m_Selecting = true;
      m_SelectionRectangle = juce::Rectangle<int>( x, y, 0, 0 );
      m_SelectionStartPoint = juce::Point<int>( x, y );

      emitSampleSelectionUpdated();
   }

   updateCursor( event );

   repaint();
}


void UISectionSamplerKeyboard::mouseUp( const MouseEvent &event )
{
   UISectionKeyboard::mouseUp( event );

   if( m_Selecting )
   {
      m_Selecting = false;
      m_SelectionRectangle = juce::Rectangle<int>( 0, 0, 0, 0 );
      m_SelectionStartPoint = juce::Point<int>( 0, 0 );
   } else
   if( m_CurrentSampleNote >= 0 )
   {
      for( SamplerEngine::Sample *pSample : m_SelectedSamples )
      {
         pSample->setMinNote( pSample->getMinNote() + m_CurrentSampleNoteOffset );
         pSample->setMaxNote( pSample->getMaxNote() + m_CurrentSampleNoteOffset );
         pSample->correctMinMaxNote();
         emitSampleSelectionUpdated();
      }

      m_CurrentSampleNote = -1;
      m_CurrentSampleNoteOffset = 0;
   }

   repaint();
}


void UISectionSamplerKeyboard::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   UISectionKeyboard::handleNoteOn( pSource, midiChannel, midiNoteNumber, velocity );
}


void UISectionSamplerKeyboard::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   UISectionKeyboard::handleNoteOff( pSource, midiChannel, midiNoteNumber, velocity );
}


std::set<SamplerEngine::Sample *> UISectionSamplerKeyboard::selectedSamples() const
{
   return( m_SelectedSamples );
}


void UISectionSamplerKeyboard::clearSelectedSamples()
{
   m_SelectedSamples.clear();
   emitSampleSelectionUpdated();
   repaint();
}


void UISectionSamplerKeyboard::emitSampleSelectionUpdated()
{
   for( UISectionSamplerKeyboardListener *pListener : m_Listeners )
   {
      pListener->onSampleSelectionUpdated( this );
   }
}


void UISectionSamplerKeyboard::emitDeleteSample( size_t nPart, SamplerEngine::Sample *pSample )
{
   for( UISectionSamplerKeyboardListener *pListener : m_Listeners )
   {
      pListener->onDeleteSample( nPart, pSample );
   }
}
