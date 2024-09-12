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
\file PluginEditor.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the plugin's GUI
*/
/*----------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <SamplerGUI/UIPageZones/UIPageZones.h>

#include "util.h"


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Constructor
*/
/*----------------------------------------------------------------------------*/
PluginEditor::PluginEditor( PluginProcessor& p )
   : AudioProcessorEditor( &p ), processorRef( p )
{
   juce::ignoreUnused (processorRef);
   // Make sure that before the constructor has finished, you've set the
   // editor's size to whatever you need it to be.
   setResizable( false, false );
   setSize( 1024, 640 );
   setResizeLimits( 1024, 640, 1024, 640 );

   m_UIPages.push_back( new SamplerGUI::UIPageZones( this ) );
   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      addAndMakeVisible( pPage );
   }

   for( int i = 0; i < SAMPLERENGINE_NUMPARTS; i++ )
   {
      juce::TextButton *pButton = new juce::TextButton( stdformat( "{}", i + 1 ) );
      pButton->setToggleable( true );
      pButton->setClickingTogglesState( true );
      pButton->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
      pButton->addListener( this );
      m_PartButtons.push_back( pButton );
      pButton->setBounds( i * 36 + 8, 7, 32, 18 );
      addAndMakeVisible( pButton );
   }

   m_pExportProgram = new juce::TextButton( "Export prg.." );
   m_pExportProgram->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pExportProgram->addListener( this );
   m_pExportProgram->setBounds( getBounds().getWidth() - 100, 7, 96, 18 );
   addAndMakeVisible( m_pExportProgram );

   m_pImportProgram = new juce::TextButton( "Import prg.." );
   m_pImportProgram->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pImportProgram->addListener( this );
   m_pImportProgram->setBounds(
      m_pExportProgram->getBounds().getX() - m_pExportProgram->getBounds().getWidth() - 4,
      m_pExportProgram->getBounds().getY(),
      m_pExportProgram->getBounds().getWidth(),
      m_pExportProgram->getBounds().getHeight()
   );
   addAndMakeVisible( m_pImportProgram );

   m_pExportMulti = new juce::TextButton( "Export multi.." );
   m_pExportMulti->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pExportMulti->addListener( this );
   m_pExportMulti->setBounds(
      m_pImportProgram->getBounds().getX() - m_pImportProgram->getBounds().getWidth() - 4,
      m_pImportProgram->getBounds().getY(),
      m_pImportProgram->getBounds().getWidth(),
      m_pImportProgram->getBounds().getHeight() );
   addAndMakeVisible( m_pExportMulti );

   m_pImportMulti = new juce::TextButton( "Import multi.." );
   m_pImportMulti->setColour( juce::TextButton::ColourIds::buttonOnColourId, juce::Colour::fromRGB( 192, 64, 64 ) );
   m_pImportMulti->addListener( this );
   m_pImportMulti->setBounds(
      m_pExportMulti->getBounds().getX() - m_pExportMulti->getBounds().getWidth() - 4,
      m_pExportMulti->getBounds().getY(),
      m_pExportMulti->getBounds().getWidth(),
      m_pExportMulti->getBounds().getHeight());
   addAndMakeVisible( m_pImportMulti );

   activatePart( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Destructor
*/
/*----------------------------------------------------------------------------*/
PluginEditor::~PluginEditor()
{
   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      delete pPage;
   }
   m_UIPages.clear();

   for( juce::TextButton *pButton : m_PartButtons )
   {
      delete pButton;
   }

   m_PartButtons.clear();

   delete m_pExportProgram;
   delete m_pImportProgram;
   delete m_pExportMulti;
   delete m_pImportMulti;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The UIPageZones object
*/
/*----------------------------------------------------------------------------*/
SamplerGUI::UIPageZones *PluginEditor::getUIPageZones() const
{
   for( size_t i = 0; i < m_UIPages.size(); i++ )
   {
      SamplerGUI::UIPageZones *pP = dynamic_cast<SamplerGUI::UIPageZones *>( m_UIPages[i] );
      if( pP )
      {
         return( pP );
      }
   }

   return( nullptr );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return All selected samples
*/
/*----------------------------------------------------------------------------*/
std::set<SamplerEngine::Sample *> PluginEditor::getSelectedSamples() const
{
   return( getUIPageZones()->getSamplerKeyboard()->selectedSamples() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return true if the solo mode is enabled
*/
/*----------------------------------------------------------------------------*/
bool PluginEditor::isSoloEnabled() const
{
   return( getUIPageZones()->isSoloEnabled() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The number of the currently selected part (0..15)
*/
/*----------------------------------------------------------------------------*/
size_t PluginEditor::currentPart() const
{
   return( m_CurrentPart );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Activate a specific part.
\param nPart The number of the part to be activated (0..15)
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::activatePart( size_t nPart )
{
   if( nPart >= m_PartButtons.size() )
      return;

   m_CurrentPart = nPart;
   for( size_t i = 0; i < m_PartButtons.size(); i++ )
   {
      m_PartButtons[i]->setToggleState( i == nPart, dontSendNotification );
   }

   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      pPage->currentPartChanged( nPart );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Callback function to register button clicks (from juce::Button::Listener)
\param pButton Pointer to the button that has been clicked
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::buttonClicked( Button *pButton )
{
   auto partIter = std::find( m_PartButtons.begin(), m_PartButtons.end(), pButton );
   if( partIter != m_PartButtons.end() )
   {
      size_t partIndex = (size_t)( partIter - m_PartButtons.begin() );
      activatePart( partIndex );
      repaint();
   } else
   if( pButton == m_pImportProgram )
   {
      juce::FileChooser ch = juce::FileChooser( "Import program..", juce::File(), "*.xml", false, true, this );
      if( ch.browseForFileToOpen( nullptr ) )
      {
         std::string fname = ch.getResult().getFullPathName().toStdString();
         std::ifstream file;
         file.open( fname, std::ifstream::in );
         if( file.is_open() )
         {
            file.seekg( 0, std::ios::end );
            size_t fsize = (size_t)file.tellg();
            file.seekg( 0, std::ios::beg );
            char *pTmp = new char[fsize];
            file.read( pTmp, (std::streamsize)fsize );
            file.close();

            xmlDocPtr doc = xmlReadMemory( pTmp, fsize, "noname.xml", nullptr, 0 );
            if( doc != nullptr )
            {
               xmlNode *pRoot = xmlDocGetRootElement( doc );
               if( pRoot )
               {
                  processor().samplerEngine()->importPart( currentPart(), pRoot );
                  repaint();
               }
               xmlFreeDoc( doc );
            }

            delete[] pTmp;
         }
      }
   } else
   if( pButton == m_pExportProgram )
   {
      juce::FileChooser ch = juce::FileChooser( "Export current program..", juce::File(), "*.xml", false, true, this );
      if( ch.browseForFileToSave( true ) )
      {
         std::string fname = ch.getResult().getFullPathName().toStdString();
         std::string partXml = util::toString( processor().samplerEngine()->getPart( currentPart() )->toXml() );

         std::ofstream file;
         file.open( fname, std::ofstream::out );
         if( file.is_open() )
         {
            file.write( partXml.c_str(), (std::streamsize)partXml.size() );
            file.close();
         }
      }
   } else
   if( pButton == m_pImportMulti )
   {
      juce::FileChooser ch = juce::FileChooser( "Import multi..", juce::File(), "*.xml", false, true, this );
      if( ch.browseForFileToOpen( nullptr ) )
      {
         std::string fname = ch.getResult().getFullPathName().toStdString();
         std::ifstream file;
         file.open( fname, std::ifstream::in );
         if( file.is_open() )
         {
            file.seekg( 0, std::ios::end );
            size_t fsize = (size_t)file.tellg();
            file.seekg( 0, std::ios::beg );
            char *pTmp = new char[fsize];
            file.read( pTmp, (std::streamsize)fsize );
            file.close();

            xmlDocPtr doc = xmlReadMemory( pTmp, fsize, "noname.xml", nullptr, 0 );
            if( doc != nullptr )
            {
               xmlNode *pRoot = xmlDocGetRootElement( doc );
               if( pRoot )
               {
                  processor().importMulti( pRoot );
                  repaint();
               }
               xmlFreeDoc( doc );
            }

            delete[] pTmp;
         }
      }
   } else
   if( pButton == m_pExportMulti )
   {
      juce::FileChooser ch = juce::FileChooser( "Export multi..", juce::File(), "*.xml", false, true, this );
      if( ch.browseForFileToSave( true ) )
      {
         std::string fname = ch.getResult().getFullPathName().toStdString();
         std::string xmlMulti = util::toString( processor().samplerEngine()->toXml() );

         std::ofstream file;
         file.open( fname, std::ofstream::out );
         if( file.is_open() )
         {
            file.write( xmlMulti.c_str(), (std::streamsize)xmlMulti.size() );
            file.close();
         }
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Callback function to register button state changed (from juce::Button::Listener)
\param pButton Pointer to the button
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::buttonStateChanged( Button */*pButton*/ )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return Reference to the PluginProcessor
*/
/*----------------------------------------------------------------------------*/
PluginProcessor &PluginEditor::processor() const
{
   return( processorRef );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Callback function to register MIDI note on events (from juce::MidiKeyboardStateListener)
\param pSource
\param midiChannel
\param midiNoteNumber
\param velocity
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Callback function to register MIDI note off events (from juce::MidiKeyboardStateListener)
\param pSource
\param midiChannel
\param midiNoteNumber
\param velocity
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
The paint event
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::paint( juce::Graphics& g )
{
   juce::ignoreUnused( g );

   g.setColour( juce::Colour::fromRGB( 0, 0, 0 ) );
   g.fillAll();
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
The resized event
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
   // subcomponents in your editor..
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This function gets called by PluginProcessor when the selection of samples has
been updated.

\param pSamplerKeyboard
*/
/*----------------------------------------------------------------------------*/
void PluginEditor::onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pSamplerKeyboard )
{
   for( SamplerGUI::UIPage *pPage : m_UIPages )
   {
      pPage->onSampleSelectionUpdated( pSamplerKeyboard );
   }
}

