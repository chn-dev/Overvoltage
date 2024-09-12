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
\file UISectionWaveView.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionWaveView.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONWAVEVIEW_H__
#define __UISECTIONWAVEVIEW_H__

#include "JuceHeader.h"

#include <SamplerEngine/Sample.h>
#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionWaveView
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
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

