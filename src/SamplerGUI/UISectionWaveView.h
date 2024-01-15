#ifndef __UISECTIONWAVEVIEW_H__
#define __UISECTIONWAVEVIEW_H__

#include "JuceHeader.h"

#include <SamplerEngine/Sample.h>

#include "UISection.h"

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   class UISectionWaveView : public UISection,
                             public juce::Button::Listener,
                             public juce::ScrollBar::Listener
   {
   public:
      UISectionWaveView( UIPage *pUIPage );
      ~UISectionWaveView();

      void paint( juce::Graphics & ) override;
      void resized() override;

      virtual void samplesUpdated();

      virtual void mouseDown( const MouseEvent &event );
      virtual void mouseDrag( const MouseEvent &event );
      virtual void mouseUp( const MouseEvent & event );
      virtual void mouseMove( const MouseEvent &event );

      virtual void buttonClicked( Button *pButton );
      virtual void buttonStateChanged( Button *pButton );

      virtual void scrollBarMoved( ScrollBar *pScrollBar, double newRangeStart );

   protected:
      int getXPosFromSampleNum( uint32_t sampleNum ) const;
      uint32_t getSampleNumFromXPos( int xPos ) const;

      uint32_t getSampleViewStart() const;
      uint32_t getSampleViewEnd() const;

   protected:
      juce::TextButton *m_pbZoom;
      juce::TextButton *m_pbShowAll;
      juce::TextButton *m_pbZoomIn;
      juce::TextButton *m_pbZoomOut;
      juce::ScrollBar *m_psScrollBar;
      uint32_t m_OrigLoopPoint;
      bool m_IsDraggingLoopStart;
      bool m_IsDraggingLoopEnd;

      bool m_IsSelecting;
      uint32_t m_SelectionStart;
      uint32_t m_SelectionEnd;

      uint32_t m_SampleViewStart;
      uint32_t m_SampleViewEnd;

   private:
   };
}
#endif
