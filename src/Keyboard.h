#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <SamplerEngine/SamplerEngine.h>

#include "JuceHeader.h"

class PluginEditor;

//==============================================================================
class Keyboard  : public juce::Component,
                  public juce::MidiKeyboardState,
                  public juce::MidiKeyboardStateListener
{
public:
   explicit Keyboard( PluginEditor *pEditor );
   ~Keyboard() override;

   //==============================================================================
   virtual void paint( juce::Graphics &g );
   virtual void resized();

   void setWidth( int w );

   void mouseMove( const MouseEvent &event) override;
   void mouseDrag( const MouseEvent &event ) override;
   void mouseEnter( const MouseEvent &event ) override;
   void mouseExit( const MouseEvent &event ) override;
   void mouseDown( const MouseEvent &event ) override;
   void mouseUp( const MouseEvent &event ) override;
   void mouseWheelMove( const MouseEvent &event, const MouseWheelDetails &wheel ) override;

   static juce::String noteName( int note );
   static juce::String noteNameWithOctave( int note );

   virtual void handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );
   virtual void handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity );

protected:
   static int keyOfs( int note );
   static int maxKeyOfs();
   static int keySize( int note );
   std::list<SamplerEngine::Sample *> &samples();
   const std::list<SamplerEngine::Sample *> &constSamples() const;

private:
   void drawNote( juce::Graphics &g, int note );
   void getNoteShape( int note, juce::Colour &c, juce::Rectangle<int> &r );
   int getNoteNumberAt( int x, int y );
   int getMaxNoteOffset() const;
   int getKeyboardHeight( int noteStart, int noteEnd ) const;

protected:
   PluginEditor *m_pEditor;
   int m_KeyHeight;
   int m_NoteOffset;
   int m_MaxNoteOffset;
   int m_Notes[16][128];
   int m_CurrentNote;
   int m_Width;
};

#endif
