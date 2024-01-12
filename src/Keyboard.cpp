#include "Keyboard.h"
#include "PluginEditor.h"

#include "util.h"

Keyboard::Keyboard( PluginEditor *pEditor ) :
   m_pEditor( pEditor ),
   m_KeyHeight( 28 ),
   m_NoteOffset( 36 ),
   m_MaxNoteOffset( 128 ),
   m_CurrentNote( -1 ),
   m_Width( -1 )
{
   for( int i = 0; i < 128; i++ )
   {
      for( int j = 0; j < 16; j++ )
      {
         m_Notes[j][i] = -1;
      }
   }
}


Keyboard::~Keyboard()
{
}


std::list<Sample *> &Keyboard::samples()
{
   return( m_pEditor->processor().samples() );
}


const std::list<Sample *> &Keyboard::constSamples() const
{
   return( m_pEditor->processor().constSamples() );
}


juce::String Keyboard::noteNameWithOctave( int note )
{
   std::string name = noteName( note ).toStdString();
   int octave = ( note / 12 ) - 2;

   int numSpaces = 0;
   if( octave >= 0 )
      numSpaces++;
   if( !name.ends_with( "#" ) )
      numSpaces++;
   for( int i = 0; i < numSpaces; i++ )
      name = name.append( " " );
   name.append( stdformat( "{}", octave ) );

   return( name );
}


juce::String Keyboard::noteName( int note )
{
   note %= 12;

   switch( note )
   {
      case 0:
         return( "C" );
      case 1:
         return( "C#" );
      case 2:
         return( "D" );
      case 3:
         return( "D#" );
      case 4:
         return( "E" );
      case 5:
         return( "F" );
      case 6:
         return( "F#" );
      case 7:
         return( "G" );
      case 8:
         return( "G#" );
      case 9:
         return( "A" );
      case 10:
         return( "A#" );
      case 11:
         return( "B" );
      default:
         return( "NIL" );
   }
}


int Keyboard::keyOfs( int note )
{
   static int ko[12] = { 0, 2, 3, 6, 7, 10, 12, 13, 16, 17, 20, 21 };
   return( ko[note % 12] );
}


int Keyboard::maxKeyOfs()
{
   return( 24 );
}


int Keyboard::keySize( int note )
{
   static int ks[12] = { 3, 2, 4, 2, 3, 3, 2, 4, 2, 4, 2, 3 };
   return( ks[note % 12] );
}


void Keyboard::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel );

   if( m_CurrentNote != midiNoteNumber )
   {
      m_Notes[midiChannel - 1][midiNoteNumber] = (int)( velocity * 128 );
      noteOn( midiChannel, midiNoteNumber, velocity );

      const MessageManagerLock mmLock;
      repaint();
   }
}


void Keyboard::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel );

   if( m_CurrentNote != midiNoteNumber )
   {
      m_Notes[midiChannel - 1][midiNoteNumber] = -1;
      noteOff( midiChannel, midiNoteNumber, velocity );

      const MessageManagerLock mmLock;
      repaint();
   }
}


void Keyboard::getNoteShape( int note, juce::Colour &c, juce::Rectangle<int> &r )
{
   int x = 0;
   int ko = keyOfs(note ) - keyOfs( m_NoteOffset );
   int octaveOfs = ( (  note / 12 ) - ( m_NoteOffset / 12 ) ) * maxKeyOfs();
   int y = getBounds().getHeight() - ( ( ( ko + octaveOfs ) * m_KeyHeight ) / 4 );
   int w = m_Width * ( keySize( note ) == 3 ? 4 : keySize( note ) ) / 4;
   int h = keySize( note ) * m_KeyHeight / 4;

   if( ( keySize( m_NoteOffset ) != 2 ) &&
       ( keySize( m_NoteOffset - 1 ) == 2 ) )
   {
      y += m_KeyHeight / 4;
   }
   y -= h;

   uint8 cv = keySize( note ) != 2 ? 255 : 0;

   c = juce::Colour::fromRGB( cv, cv, cv );
   r = juce::Rectangle<int>( x, y, w, h );
}

void Keyboard::drawNote( juce::Graphics &g, int note )
{
   juce::Colour c;
   juce::Rectangle<int> r;
   getNoteShape( note, c, r );

   if( r.getY() + r.getHeight() < 0 )
      return;

   if( m_Notes[m_pEditor->currentPart()][note] >= 0 )
      g.setColour( juce::Colour::fromRGB( 255, 64, 64 ) );
   else
      g.setColour( c );
   g.fillRect( r );

   g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
   g.drawRect( r );

   if( note % 12 == 0 )
   {
      // C
      std::string t = stdformat( "{} " , ( note / 12 ) - 2 );
      g.setFont( r.getHeight() * 0.85f );
      g.drawText( t, r, juce::Justification::right );
   }
}


void Keyboard::paint( juce::Graphics &g )
{
   g.setColour( juce::Colour::fromRGB( 16, 32, 16 ) );
   g.fillAll();

   g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
   g.drawRect( 0, 0, m_Width, getBounds().getHeight() );

   for( int note = m_NoteOffset; note <= 128; note++ )
   {
      if( keySize( note ) != 2 ) // White key
      {
         drawNote( g, note );
      }
   }

   for( int note = m_NoteOffset; note <= 128; note++ )
   {
      if( keySize( note ) == 2 ) // Black key
      {
         drawNote( g, note );
      }
   }
}


void Keyboard::resized()
{
   m_MaxNoteOffset = getMaxNoteOffset();
   if( m_Width < 0 )
   {
      m_Width = getBounds().getWidth();
   }
}


void Keyboard::setWidth( int w )
{
   if( m_Width != w )
   {
      m_Width = w;
      repaint();
   }
}


int Keyboard::getMaxNoteOffset() const
{
   for( int noteStart = 0; noteStart <= 128; noteStart++ )
   {
      if( getKeyboardHeight( noteStart, 128 ) <= getBounds().getHeight() )
      {
         return( noteStart );
      }
   }

   return( 0 );
}


int Keyboard::getKeyboardHeight( int noteStart, int noteEnd ) const
{
   int ko = keyOfs( noteStart ) - keyOfs( m_NoteOffset );
   int octaveOfs = ( ( noteStart / 12 ) - ( m_NoteOffset / 12 ) ) * maxKeyOfs();
   int h1 = ( ( ko + octaveOfs ) * m_KeyHeight ) / 4;

   ko = keyOfs( noteEnd + 1 ) - keyOfs( m_NoteOffset );
   octaveOfs = ( ( ( noteEnd + 1 ) / 12 ) - ( m_NoteOffset / 12 ) ) * maxKeyOfs();
   int h2 = ( ( ko + octaveOfs ) * m_KeyHeight ) / 4;

   return( h2 - h1 );
}


int Keyboard::getNoteNumberAt( int x, int y )
{
   juce::Colour c;
   juce::Rectangle<int> r;

   for( int note = m_NoteOffset; note <= 128; note++ )
   {
      if( keySize( note ) == 2 ) // Black notes
      {
         getNoteShape( note, c, r );
         if( x >= r.getX() && y >= r.getY() &&
             x < r.getX() + r.getWidth() && y < r.getY() + r.getHeight() )
         {
            return( note );
         }
      }
   }

   for( int note = m_NoteOffset; note <= 128; note++ )
   {
      if( keySize( note ) != 2 ) // White notes
      {
         getNoteShape( note, c, r );
         if( x >= r.getX() && y >= r.getY() &&
             x < r.getX() + r.getWidth() && y < r.getY() + r.getHeight() )
         {
            return( note );
         }
      }
   }


   return( -1 );
}

void Keyboard::mouseDrag( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   int note = getNoteNumberAt( x, y );

   if( m_CurrentNote >= 0 )
   {
      if( m_CurrentNote != note )
      {
         m_Notes[m_pEditor->currentPart()][m_CurrentNote] = -1;
         noteOff( (int)m_pEditor->currentPart() + 1, m_CurrentNote, 1.0 );

         m_CurrentNote = -1;

         if( note >= 0 )
         {
            m_Notes[m_pEditor->currentPart()][note] = 127;
            m_CurrentNote = note;
            noteOn( (int)m_pEditor->currentPart() + 1, m_CurrentNote, 1.0 );
         }

         repaint();
      }
   }
}

void Keyboard::mouseMove( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   int note = getNoteNumberAt( x, y );

   juce::ignoreUnused( note );
}


void Keyboard::mouseEnter( const MouseEvent &event )
{
   juce::ignoreUnused( event );
}


void Keyboard::mouseExit( const MouseEvent &event )
{
   juce::ignoreUnused( event );
}


void Keyboard::mouseDown( const MouseEvent &event )
{
   int x = event.getMouseDownPosition().getX();
   int y = event.getMouseDownPosition().getY();

   int note = getNoteNumberAt( x, y );

   if( note >= 0 )
   {
      if( m_Notes[m_pEditor->currentPart()][note] < 0 )
      {
         m_Notes[m_pEditor->currentPart()][note] = 127;
         m_CurrentNote = note;
         noteOn( (int)m_pEditor->currentPart() + 1, note, 1.0 );
         repaint();
      }
   }
}


void Keyboard::mouseUp( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   int note = getNoteNumberAt( x, y );

   if( m_CurrentNote >= 0 )
   {
      m_Notes[m_pEditor->currentPart()][m_CurrentNote] = -1;
      m_CurrentNote = -1;
      noteOff( (int)m_pEditor->currentPart() + 1, note, 1.0 );
      repaint();
   }
}


void Keyboard::mouseWheelMove( const MouseEvent &event, const MouseWheelDetails &wheel )
{
   juce::ignoreUnused( event );

   double dy = wheel.deltaY;

   int bak = m_NoteOffset;

   if( dy > 0.0 )
   {
      do
      {
         m_NoteOffset++;
      } while( keySize( m_NoteOffset ) == 2 );
   } else
   if( dy < 0.0 )
   {
      do
      {
         m_NoteOffset--;
      } while( keySize( m_NoteOffset ) == 2 );
   }

   if( m_NoteOffset < 0 ||
       m_NoteOffset > m_MaxNoteOffset )
      m_NoteOffset = bak;

   repaint();
}
