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
\file UISectionModMatrix.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class UISectionModMatrix.
*/
/*----------------------------------------------------------------------------*/
#ifndef __UISECTIONMODMATRIX_H__
#define __UISECTIONMODMATRIX_H__

#include "JuceHeader.h"

#include <SamplerGUI/UISection.h>

class PluginEditor;

//==============================================================================
namespace SamplerGUI
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class UISectionModMatrix
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class UISectionModMatrix : public UISection
   {
   public:
      class UIModSlot : public juce::Label::Listener,
                        public juce::ComboBox::Listener,
                        public juce::Button::Listener

      {
      public:
         UIModSlot();
         UIModSlot( UISectionModMatrix *pSectionModMatrix, SamplerEngine::ModMatrix::ModSlot *pModSlot, int index, int xp, int yp );
         ~UIModSlot();

         SamplerEngine::ModMatrix::ModSlot *getModSlot() const;
         virtual void comboBoxChanged( ComboBox *pComboBox );
         virtual void buttonClicked( Button *pButton );
         virtual void buttonStateChanged( Button *pButton );
         virtual void labelTextChanged( Label *pLabel );

      private:
         UISectionModMatrix *m_pSectionModMatrix;
         juce::ComboBox *m_pcbSrc;
         juce::ComboBox *m_pcbMod;
         juce::ComboBox *m_pcbDest;
         juce::TextButton *m_pbEnabled;
         CycleComponent *m_pccAmount;
         juce::ComboBox *m_pcbMathFunc;
         SamplerEngine::ModMatrix::ModSlot *m_pModSlot;
         int m_Index;
         int m_Xp;
         int m_Yp;
      };

      UISectionModMatrix( UIPage *pUIPage );
      ~UISectionModMatrix();

      virtual void paint( juce::Graphics &g );
      virtual void resized();

      virtual void samplesUpdated();

   protected:
      std::vector<UIModSlot *> m_UIModSlots;
   };
}

#endif

