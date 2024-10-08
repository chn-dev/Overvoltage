/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of chn's Overvoltage.                                   *
 *                                                                             *
 *  Overvoltage is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *          
 *  Overvoltage is distributed in the hope that it will be useful, but         * 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Overvoltage. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file UISection.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements a Keyboard UI section
*/
/*----------------------------------------------------------------------------*/
#include "UISectionKeyboard.h"
#include "PluginEditor.h"
#include <SamplerGUI/UIPageZones/UIPageZones.h>

#include "util.h"

using namespace SamplerGUI;


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Constructor
*/
/*----------------------------------------------------------------------------*/
UISectionKeyboard::UISectionKeyboard( UIPageZones *pPageZones ) :
   UISection( pPageZones ),
   m_pPageZones( pPageZones ),
   m_KeyHeight( 28 ),
   m_NoteOffset( 36 ),
   m_MaxNoteOffset( 128 ),
   m_CurrentNote( -1 ),
   m_Width( -1 )
{
   for( int i = 0; i < 128; i++ )
   {
      for( int j = 0; j < SAMPLERENGINE_NUMPARTS; j++ )
      {
         m_Notes[j][i] = -1;
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Destructor
*/
/*----------------------------------------------------------------------------*/
UISectionKeyboard::~UISectionKeyboard()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A list of all samples
*/
/*----------------------------------------------------------------------------*/
std::list<SamplerEngine::Sample *> &UISectionKeyboard::samples()
{
   return( m_pPageZones->editor()->processor().samples() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
\return A list of all samples
*/
/*----------------------------------------------------------------------------*/
const std::list<SamplerEngine::Sample *> &UISectionKeyboard::constSamples() const
{
   return( m_pPageZones->editor()->processor().constSamples() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Convert a MIDI note numer to a string like "C#3" or "F-2".
\param note The MIDI note number
\return The note string
*/
/*----------------------------------------------------------------------------*/
juce::String UISectionKeyboard::noteNameWithOctave( int note )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Convert a MIDI note number to a note name without octave.
\param note The MIDI note number
\return The note string
*/
/*----------------------------------------------------------------------------*/
juce::String UISectionKeyboard::noteName( int note )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
int UISectionKeyboard::keyOfs( int note )
{
   static int ko[12] = { 0, 2, 3, 6, 7, 10, 12, 13, 16, 17, 20, 21 };
   return( ko[note % 12] );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
int UISectionKeyboard::maxKeyOfs()
{
   return( 24 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
int UISectionKeyboard::keySize( int note )
{
   static int ks[12] = { 3, 2, 4, 2, 3, 3, 2, 4, 2, 4, 2, 3 };
   return( ks[note % 12] );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function for handing the note on event.
\param pSource
\param midiChannel
\param midiNoteNumber
\param velocity
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function for handling the note off event.
\param pSource
\param midiChannel
\param midiNoteNumber
\param velocity
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::getNoteShape( int note, juce::Colour &c, juce::Rectangle<int> &r )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::drawNote( juce::Graphics &g, int note )
{
   juce::Colour c;
   juce::Rectangle<int> r;
   getNoteShape( note, c, r );

   if( r.getY() + r.getHeight() < 0 )
      return;

   if( m_Notes[m_pPageZones->editor()->currentPart()][note] >= 0 )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::paint( juce::Graphics &g )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::resized()
{
   m_MaxNoteOffset = getMaxNoteOffset();
   if( m_Width < 0 )
   {
      m_Width = getBounds().getWidth();
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::setWidth( int w )
{
   if( m_Width != w )
   {
      m_Width = w;
      repaint();
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
int UISectionKeyboard::getMaxNoteOffset() const
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Used for painting the keyboard
*/
/*----------------------------------------------------------------------------*/
int UISectionKeyboard::getKeyboardHeight( int noteStart, int noteEnd ) const
{
   int ko = keyOfs( noteStart ) - keyOfs( m_NoteOffset );
   int octaveOfs = ( ( noteStart / 12 ) - ( m_NoteOffset / 12 ) ) * maxKeyOfs();
   int h1 = ( ( ko + octaveOfs ) * m_KeyHeight ) / 4;

   ko = keyOfs( noteEnd + 1 ) - keyOfs( m_NoteOffset );
   octaveOfs = ( ( ( noteEnd + 1 ) / 12 ) - ( m_NoteOffset / 12 ) ) * maxKeyOfs();
   int h2 = ( ( ko + octaveOfs ) * m_KeyHeight ) / 4;

   return( h2 - h1 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Retrieve the MIDI note number from a mouse position.
\param x
\param y
\return The MIDI note number
*/
/*----------------------------------------------------------------------------*/
int UISectionKeyboard::getNoteNumberAt( int x, int y )
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


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseDrag( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   int note = getNoteNumberAt( x, y );

   if( m_CurrentNote >= 0 )
   {
      if( m_CurrentNote != note )
      {
         m_Notes[m_pPageZones->editor()->currentPart()][m_CurrentNote] = -1;
         noteOff( (int)m_pPageZones->editor()->currentPart() + 1, m_CurrentNote, 1.0 );

         m_CurrentNote = -1;

         if( note >= 0 )
         {
            int velocity = 127;
            m_Notes[m_pPageZones->editor()->currentPart()][note] = velocity;
            m_CurrentNote = note;
            noteOn( (int)m_pPageZones->editor()->currentPart() + 1, m_CurrentNote, (float)velocity / 127.0f );
         }

         repaint();
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseMove( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   int note = getNoteNumberAt( x, y );

   juce::ignoreUnused( note );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseEnter( const MouseEvent &event )
{
   juce::ignoreUnused( event );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseExit( const MouseEvent &event )
{
   juce::ignoreUnused( event );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseDown( const MouseEvent &event )
{
   int x = event.getMouseDownPosition().getX();
   int y = event.getMouseDownPosition().getY();

   int note = getNoteNumberAt( x, y );

   if( note >= 0 )
   {
      if( m_Notes[m_pPageZones->editor()->currentPart()][note] < 0 )
      {
         int velocity = 127;
         m_Notes[m_pPageZones->editor()->currentPart()][note] = velocity;
         m_CurrentNote = note;
         noteOn( (int)m_pPageZones->editor()->currentPart() + 1, note, (float)velocity / 127.0f );
         repaint();
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseUp( const MouseEvent &event )
{
   int x = event.getPosition().getX();
   int y = event.getPosition().getY();

   int note = getNoteNumberAt( x, y );

   if( m_CurrentNote >= 0 )
   {
      m_Notes[m_pPageZones->editor()->currentPart()][m_CurrentNote] = -1;
      m_CurrentNote = -1;
      noteOff( (int)m_pPageZones->editor()->currentPart() + 1, note, 1.0 );
      repaint();
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-28
Callback function from juce::Component
*/
/*----------------------------------------------------------------------------*/
void UISectionKeyboard::mouseWheelMove( const MouseEvent &event, const MouseWheelDetails &wheel )
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

