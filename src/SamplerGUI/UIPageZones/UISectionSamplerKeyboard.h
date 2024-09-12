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
\file UISectionSamplerKeyboard.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionSamplerKeyboard.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONSAMPLERKEYBOARD_H__
#define __UISECTIONSAMPLERKEYBOARD_H__

#include <list>
#include <set>

#include <SamplerEngine/Sample.h>

#include "JuceHeader.h"
#include "UISectionKeyboard.h"

namespace SamplerGUI
{
   class UISectionSamplerKeyboard;
   class UISectionKeyboard;
   class UIPage;

   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionSamplerKeyboardListener
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionSamplerKeyboardListener
   {
   public:
      virtual void onDeleteSample( size_t part, SamplerEngine::Sample *pSample ) = 0;
      virtual void onSampleSelectionUpdated( UISectionSamplerKeyboard *pKeyboard ) = 0;
   };


   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionSamplerKeyboard
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionSamplerKeyboard : public UISectionKeyboard,
                                    public juce::FileDragAndDropTarget,
                                    public juce::KeyListener

   {
   public:
      UISectionSamplerKeyboard( UIPageZones *pPageZones );
      ~UISectionSamplerKeyboard();

      //==============================================================================
      virtual void paint( juce::Graphics &g ) override;

      virtual void samplesUpdated();

      void addSamplerKeyboardListener( UISectionSamplerKeyboardListener *pListener );

      virtual void mouseMove( const MouseEvent &event);
      virtual void mouseDrag( const MouseEvent &event );
      virtual void mouseEnter( const MouseEvent &event );
      virtual void mouseExit( const MouseEvent &event );
      virtual void mouseDown( const MouseEvent &event );
      virtual void mouseUp( const MouseEvent &event );

      virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
      virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

      virtual bool keyPressed( const KeyPress &key );
      virtual bool keyPressed( const KeyPress &key, Component *pOriginatingComponent );
      virtual bool keyStateChanged( bool isKeyDown );
      virtual bool keyStateChanged( bool isKeyDown, Component *pOriginatingComponent );

      virtual bool isInterestedInFileDrag( const StringArray &files );
      virtual void fileDragEnter( const StringArray &files, int x, int y );
      virtual void fileDragMove( const StringArray &files, int x, int y );
      virtual void fileDragExit( const StringArray &files );
      virtual void filesDropped( const StringArray &files, int x, int y );

      std::set<SamplerEngine::Sample *> selectedSamples() const;
      void clearSelectedSamples();
      void selectAll();
      void selectLayer( int nLayer = -1 );

   protected:
      int dragDropNote( int y ) const;
      int dragDropVelocity( int x ) const;
      SamplerEngine::Sample *getSampleAt( int x, int y ) const;
      juce::Rectangle<int> getNoteRect( SamplerEngine::Sample *const pSample ) const;
      juce::Rectangle<int> getNoteRect( int minNote, int maxNote, int minVel, int maxVel ) const;
      bool drawSample( juce::Graphics &g, SamplerEngine::Sample *const pSample ) const;

   private:
      void updateCursor( const MouseEvent &event );
      void emitSampleSelectionUpdated();
      void emitDeleteSample( size_t nPart, SamplerEngine::Sample *pSample );
      void deleteSelectedSamples();

   private:
      std::set<SamplerEngine::Sample *> m_SelectedSamples;
      std::set<SamplerEngine::Sample *> m_AddSelectedSamples;
      std::vector<UISectionSamplerKeyboardListener *> m_Listeners;

      SamplerEngine::Sample *m_pCurrentSample;
      int m_CurrentSampleNote;
      int m_CurrentSampleNoteOffset;
      int m_CurrentSampleHandle;

      int m_DragDropNote;

      juce::Point<int> m_SelectionStartPoint;
      juce::Rectangle<int> m_SelectionRectangle;
      bool m_Selecting;
      bool m_AddSamples;
   };
}

#endif

