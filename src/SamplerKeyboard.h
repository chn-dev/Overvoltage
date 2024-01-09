#pragma once

#include <list>
#include <set>

#include "JuceHeader.h"
#include "Keyboard.h"
#include "Sample.h"

class SamplerKeyboardListener
{
public:
   virtual void onDeleteSample( int part, Sample *pSample ) = 0;
};

//==============================================================================
class SamplerKeyboard : public Keyboard,
                        public juce::FileDragAndDropTarget,
                        public juce::KeyListener

{
public:
   SamplerKeyboard( AudioPluginAudioProcessorEditor *pEditor );
   ~SamplerKeyboard();

   //==============================================================================
   virtual void paint( juce::Graphics &g ) override;

   void addSamplerKeyboardListener( SamplerKeyboardListener *pListener );

   virtual void mouseMove( const MouseEvent &event);
   virtual void mouseDrag( const MouseEvent &event );
   virtual void mouseEnter( const MouseEvent &event );
   virtual void mouseExit( const MouseEvent &event );
   virtual void mouseDown( const MouseEvent &event );
   virtual void mouseUp( const MouseEvent &event );

   virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
   virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

   virtual bool keyPressed( const KeyPress &key, Component *pOriginatingComponent );
   virtual bool keyStateChanged( bool isKeyDown, Component *pOriginatingComponent );

   virtual bool isInterestedInFileDrag( const StringArray &files );
   virtual void fileDragEnter( const StringArray &files, int x, int y );
   virtual void fileDragMove( const StringArray &files, int x, int y );
   virtual void fileDragExit( const StringArray &files );
   virtual void filesDropped( const StringArray &files, int x, int y );

   std::set<Sample *> selectedSamples() const;
   void clearSelectedSamples();

protected:
   int dragDropNote( int y ) const;
   int dragDropVelocity( int x ) const;
   Sample *getSampleAt( int x, int y ) const;
   juce::Rectangle<int> getNoteRect( Sample *const pSample ) const;
   juce::Rectangle<int> getNoteRect( int minNote, int maxNote, int minVel, int maxVel ) const;
   bool drawSample( juce::Graphics &g, Sample *const pSample ) const;

private:
   void updateCursor( const MouseEvent &event );

private:
   std::set<Sample *> m_SelectedSamples;
   std::vector<SamplerKeyboardListener *> m_Listeners;

   Sample *m_pCurrentSample;
   int m_CurrentSampleNote;
   int m_CurrentSampleNoteOffset;
   int m_CurrentSampleHandle;

   int m_DragDropNote;

   juce::Point<int> m_SelectionStartPoint;
   juce::Rectangle<int> m_SelectionRectangle;
   bool m_Selecting;
};
