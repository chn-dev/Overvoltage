/*----------------------------------------------------------------------------*/
/*!
\file PluginProcessor.cpp
\author Christian Nowak <chnowak@web.de>
\brief This class implements the plugin's processor
*/
/*----------------------------------------------------------------------------*/
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "util.h"

#include <libxml/parser.h>
#include <libxml/tree.h>


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Constructor
*/
/*----------------------------------------------------------------------------*/
PluginProcessor::PluginProcessor()
   : AudioProcessor( BusesProperties()
      .withOutput( "Output 1", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 2", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 3", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 4", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 5", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 6", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 7", juce::AudioChannelSet::stereo(), true )
      .withOutput( "Output 8", juce::AudioChannelSet::stereo(), true )
   ), m_pEditor( nullptr )
{
   m_pEngine = new SamplerEngine::Engine( this );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Destructor
*/
/*----------------------------------------------------------------------------*/
PluginProcessor::~PluginProcessor()
{
   delete m_pEngine;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The plugin name
*/
/*----------------------------------------------------------------------------*/
const juce::String PluginProcessor::getName() const
{
   return JucePlugin_Name;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return true if the plugin accepts MIDI input
*/
/*----------------------------------------------------------------------------*/
bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
   return( true );
#else
   return( false );
#endif
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return true if the plugin generates MIDI output
*/
/*----------------------------------------------------------------------------*/
bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
   return( true );
#else
   return( false );
#endif
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return true if the plugin is a MIDI processor
*/
/*----------------------------------------------------------------------------*/
bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
   return( true );
#else
   return( false );
#endif
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
double PluginProcessor::getTailLengthSeconds() const
{
   return( 0.0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The number of available programs
*/
/*----------------------------------------------------------------------------*/
int PluginProcessor::getNumPrograms()
{
   return( 1 ); // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The current progrm number
*/
/*----------------------------------------------------------------------------*/
int PluginProcessor::getCurrentProgram()
{
   return( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
Change the current program.
\param index The number of the new program
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::setCurrentProgram( int index )
{
   juce::ignoreUnused( index );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\param index The program number
\return The specified program's name
*/
/*----------------------------------------------------------------------------*/
const juce::String PluginProcessor::getProgramName( int index )
{
   juce::ignoreUnused( index );
   return( "Overvoltage" );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\param index The program number
\param newName The new name for the specified program
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::changeProgramName( int index, const juce::String &newName )
{
   juce::ignoreUnused( index, newName );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\param sampleRate
\param samplesPerBlock
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::prepareToPlay( double sampleRate, int samplesPerBlock )
{
   m_sampleRate = sampleRate;
   m_samplesPerBlock = samplesPerBlock;
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::releaseResources()
{
   // When playback stops, you can use this as an opportunity to free up any
   // spare memory, etc.
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
bool PluginProcessor::isBusesLayoutSupported( const BusesLayout& layouts ) const
{
   // This is the place where you check if the layout is supported.
   // In this template code we only support mono or stereo.
   // Some plugin hosts, such as certain GarageBand versions, will only
   // load plugins that support stereo bus layouts.
   if( layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
       layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo() )
      return( false );

   return( true );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return A list of all samples in the currently selected part
*/
/*----------------------------------------------------------------------------*/
std::list<SamplerEngine::Sample *> &PluginProcessor::samples()
{
   return( m_pEngine->samples( m_pEditor->currentPart() ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return A list of all samples in the currently selected part
*/
/*----------------------------------------------------------------------------*/
const std::list<SamplerEngine::Sample *> &PluginProcessor::constSamples() const
{
   return( m_pEngine->constSamples( m_pEditor->currentPart() ) );
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
void PluginProcessor::handleNoteOn( MidiKeyboardState */*pSource*/, int midiChannel, int midiNoteNumber, float velocity )
{
   int vel = (int)( 127 * velocity );
   m_pEngine->noteOn( (size_t)( midiChannel - 1 ), midiNoteNumber, vel );
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
void PluginProcessor::handleNoteOff( MidiKeyboardState */*pSource*/, int midiChannel, int midiNoteNumber, float velocity )
{
   int vel = (int)( 127 * velocity );
   m_pEngine->noteOff( (size_t)( midiChannel - 1), midiNoteNumber, vel );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This function gets called when the MIDI keyboard's pitch wheel position has changed.

\param midiChannel The midi channel (1..16)
\param v The pitch wheel position (-1..1)
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::handlePitchbend( int midiChannel, double v )
{
   m_pEngine->pitchbend( (size_t)( midiChannel - 1 ), v );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This function gets called when a MIDI controller has changed.

\param midiChannel The midi channel (1..16)
\param ccNum The controller's number (0..255)
\param v The controller's value (-1..1)
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::handleControllerChange( int midiChannel, int ccNum, double v )
{
   m_pEngine->controllerChange( (size_t)( midiChannel - 1 ), ccNum, v );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
bool PluginProcessor::outputBusReady( juce::AudioBuffer<float>& buffer, int n ) const
{
   if( n >= getBusCount( false ) )
   {
      return( false );
   }

   const Bus *pBus = getBus( false, n );
   if( !pBus->isEnabled() )
   {
      return( false );
   }

   AudioBuffer<float> buf = pBus->getBusBuffer( buffer );
   if( buf.getNumChannels() != 2 )
   {
      return( false );
   }

   return( true );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This function gets called by the host when new audio shall be generated.
\param buffer
\param midiMessages
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::processBlock( juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages )
{
   double bpm = 120.0;
   if( auto p = getPlayHead()->getPosition() )
   {
      if( auto pBpm = p->getBpm() )
      {
         bpm = *pBpm;
      }
   }

   for (const juce::MidiMessageMetadata metadata : midiMessages)
   {
      const juce::MidiMessage msg = metadata.getMessage();
      if( msg.isNoteOn() )
      {
         noteOn( msg.getChannel(), msg.getNoteNumber(), msg.getVelocity() / 127.0f );
      } else
      if( msg.isNoteOff() )
      {
         noteOff( msg.getChannel(), msg.getNoteNumber(), msg.getVelocity() / 127.0f );
      } else
      if( msg.isPitchWheel() )
      {
         int pitchValue = msg.getPitchWheelValue();
         double v = ( 2.0 * ( (double)pitchValue / (double)0x3fff ) ) - 1.0;
         handlePitchbend( msg.getChannel(), v );
      } else
      if( msg.isController() )
      {
         // ccNum == 1 -> modwheel
         int ccNum = msg.getControllerNumber();
         // ccVal = 0..127
         int ccVal = msg.getControllerValue();
         double v = (double)ccVal / 127.0;
         handleControllerChange( msg.getChannel(), ccNum, v );
      }
   }

   midiMessages.clear();

   juce::ScopedNoDenormals noDenormals;
   auto totalNumInputChannels  = getTotalNumInputChannels();
   auto totalNumOutputChannels = getTotalNumOutputChannels();

   // In case we have more outputs than inputs, this code clears any output
   // channels that didn't contain input data, (because these aren't
   // guaranteed to be empty - they may contain garbage).
   // This is here to avoid people getting screaming feedback
   // when they first compile a plugin, but obviously you don't need to keep
   // this code if your algorithm always overwrites all the output channels.
   for( auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i )
   {
      buffer.clear (i, 0, buffer.getNumSamples());
   }

   // This is the place where you'd normally do the guts of your plugin's
   // audio processing...
   // Make sure to reset the state if your inner loop is processing
   // the samples and the outer loop is handling the channels.
   // Alternatively, you can process the samples with the channels
   // interleaved by keeping the same state.
   std::vector<SamplerEngine::OutputBus> buses;
   buses.resize( (size_t)getBusCount( false ) );

   for( int i = 0; i < getBusCount( false ); i++ )
   {
      const Bus *pBus = getBus( false, i );

      if( pBus->isEnabled() && outputBusReady( buffer, i ) )
      {
         AudioBuffer b = pBus->getBusBuffer( buffer );
         std::vector<float *> channelWritePointers;
         for( int channel = 0; channel < b.getNumChannels(); channel++ )
         {
            float *pChannelData = b.getWritePointer( channel );
            channelWritePointers.push_back( pChannelData );
            for( int j = 0; j < b.getNumSamples(); j++ )
            {
               pChannelData[j] = 0.0;
            }
         }
         buses[(size_t)i] = SamplerEngine::OutputBus( (size_t)b.getNumSamples(), channelWritePointers );
      }
   }

   if( m_pEngine->process( buses, m_sampleRate, bpm ) )
   {
      if( m_pEditor )
      {
         const MessageManagerLock mmLock;
         m_pEditor->repaint();
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return true if the plugin has an editor
*/
/*----------------------------------------------------------------------------*/

bool PluginProcessor::hasEditor() const
{
   return( true ); // (change this to false if you choose to not supply an editor)
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return Pointer to the plugin editor
*/
/*----------------------------------------------------------------------------*/
juce::AudioProcessorEditor *PluginProcessor::createEditor()
{
   m_pEditor = new PluginEditor( *this );
   return( m_pEditor );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This function gets called when by the host retrieve the plugin state.
\param destData That's where the state is saved.
*/
/*----------------------------------------------------------------------------*/

void PluginProcessor::getStateInformation( juce::MemoryBlock& destData )
{
   // You should use this method to store your parameters in the memory block.
   // You could do that either as raw data, or use the XML or ValueTree classes
   // as intermediaries to make it easy to save and load complex data.
   std::string xml = util::toString( m_pEngine->toXml() );
   destData.replaceAll( xml.c_str(), xml.length() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This function gets by the host to restore a previously saved plugin state.
\param data Pointer to the data
\param sizeInBytes Data size in bytes
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::setStateInformation( const void* data, int sizeInBytes )
{
   // You should use this method to restore your parameters from this memory block,
   // whose contents will have been created by the getStateInformation() call.
   xmlDocPtr doc = xmlReadMemory( (const char *)data, sizeInBytes, "noname.xml", nullptr, 0 );
   if( doc != nullptr )
   {
      xmlNode *pRoot = xmlDocGetRootElement( doc );
      SamplerEngine::Engine *pEngine = SamplerEngine::Engine::fromXml( pRoot );
      if( pEngine )
      {
         delete m_pEngine;
         pEngine->setProcessor( this );
         m_pEngine = pEngine;
      }

      xmlFreeDoc( doc );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return A new instance of the plugin
*/
/*----------------------------------------------------------------------------*/
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
   return( new PluginProcessor() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This callback function get called when a specific sample shall be deleted.
(from SamplerGUI::UISectionSamplerKeyboardListener)
\param part The part number (0..15)
\param pSample Pointer to the sample to be deleted
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::onDeleteSample( size_t part, SamplerEngine::Sample *pSample )
{
   m_pEngine->deleteSample( part, pSample );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
This callback function get called when a the selection of samples has changed.
(from SamplerGUI::UISectionSamplerKeyboardListener)
\param pKeyboard
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pKeyboard )
{
   if( m_pEditor )
   {
      m_pEditor->onSampleSelectionUpdated( pKeyboard );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The Engine
*/
/*----------------------------------------------------------------------------*/
SamplerEngine::Engine *PluginProcessor::samplerEngine() const
{
   return( m_pEngine );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
\return The PluginEditor
*/
/*----------------------------------------------------------------------------*/
PluginEditor *PluginProcessor::pluginEditor() const
{
   return( m_pEditor );
}


/*----------------------------------------------------------------------------*/
/*! 2024-06-10
*/
/*----------------------------------------------------------------------------*/
void PluginProcessor::importMulti( xmlNode *pXmlMulti )
{
   SamplerEngine::Engine *pEngine = SamplerEngine::Engine::fromXml( pXmlMulti );
   if( pEngine )
   {
      delete m_pEngine;
      m_pEngine = pEngine;
   }
}

