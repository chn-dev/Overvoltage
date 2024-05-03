#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
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
   m_pEngine = new SamplerEngine::Engine();
}


PluginProcessor::~PluginProcessor()
{
   delete m_pEngine;
}


//==============================================================================
const juce::String PluginProcessor::getName() const
{
   return JucePlugin_Name;
}


bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
   return( true );
#else
   return( false );
#endif
}


bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
   return( true );
#else
   return( false );
#endif
}


bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
   return( true );
#else
   return( false );
#endif
}


double PluginProcessor::getTailLengthSeconds() const
{
   return( 0.0 );
}


int PluginProcessor::getNumPrograms()
{
   return( 1 ); // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}


int PluginProcessor::getCurrentProgram()
{
   return( 0 );
}


void PluginProcessor::setCurrentProgram( int index )
{
   juce::ignoreUnused( index );
}


const juce::String PluginProcessor::getProgramName( int index )
{
   juce::ignoreUnused( index );
   return {};
}


void PluginProcessor::changeProgramName( int index, const juce::String &newName )
{
   juce::ignoreUnused( index, newName );
}


//==============================================================================
void PluginProcessor::prepareToPlay( double sampleRate, int samplesPerBlock )
{
   m_sampleRate = sampleRate;
   m_samplesPerBlock = samplesPerBlock;
}


void PluginProcessor::releaseResources()
{
   // When playback stops, you can use this as an opportunity to free up any
   // spare memory, etc.
}


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

std::list<SamplerEngine::Sample *> &PluginProcessor::samples()
{
   return( m_pEngine->samples( m_pEditor->currentPart() ) );
}


const std::list<SamplerEngine::Sample *> &PluginProcessor::constSamples() const
{
   return( m_pEngine->constSamples( m_pEditor->currentPart() ) );
}


void PluginProcessor::handleNoteOn( MidiKeyboardState */*pSource*/, int midiChannel, int midiNoteNumber, float velocity )
{
   int vel = (int)( 127 * velocity );
   m_pEngine->noteOn( (size_t)( midiChannel - 1 ), midiNoteNumber, vel );
}


void PluginProcessor::handleNoteOff( MidiKeyboardState */*pSource*/, int midiChannel, int midiNoteNumber, float velocity )
{
   int vel = (int)( 127 * velocity );
   m_pEngine->noteOff( (size_t)( midiChannel - 1), midiNoteNumber, vel );
}


void PluginProcessor::handlePitchbend( int midiChannel, double v )
{
   m_pEngine->pitchbend( (size_t)( midiChannel - 1 ), v );
}


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


void PluginProcessor::processBlock( juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages )
{
   double bpm = 120.0;
   bool hasBpm = false;
   if( auto p = getPlayHead()->getPosition() )
   {
      if( auto pBpm = p->getBpm() )
      {
         hasBpm = true;
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
         // int ccNum = msg.getControllerNumber();
         // ccVal = 0..127
         // int ccVal = msg.getControllerValue();
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


//==============================================================================
bool PluginProcessor::hasEditor() const
{
   return( true ); // (change this to false if you choose to not supply an editor)
}


juce::AudioProcessorEditor *PluginProcessor::createEditor()
{
   m_pEditor = new PluginEditor( *this );
   return( m_pEditor );
}


//==============================================================================
void PluginProcessor::getStateInformation( juce::MemoryBlock& destData )
{
   // You should use this method to store your parameters in the memory block.
   // You could do that either as raw data, or use the XML or ValueTree classes
   // as intermediaries to make it easy to save and load complex data.
   juce::XmlElement *pOvervoltage = m_pEngine->toXml();
   copyXmlToBinary( *pOvervoltage, destData );
   delete pOvervoltage;
}


void PluginProcessor::setStateInformation( const void* data, int sizeInBytes )
{
   // You should use this method to restore your parameters from this memory block,
   // whose contents will have been created by the getStateInformation() call.
   std::unique_ptr<juce::XmlElement> xmlState( getXmlFromBinary( data, sizeInBytes ) );
   if( xmlState.get() )
   {
      SamplerEngine::Engine *pEngine = SamplerEngine::Engine::fromXml( xmlState.get() );
      if( pEngine )
      {
         delete m_pEngine;
         m_pEngine = pEngine;
      }
   }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
   return( new PluginProcessor() );
}


void PluginProcessor::onDeleteSample( size_t part, SamplerEngine::Sample *pSample )
{
   m_pEngine->deleteSample( part, pSample );
}


void PluginProcessor::onSampleSelectionUpdated( SamplerGUI::UISectionSamplerKeyboard *pKeyboard )
{
   if( m_pEditor )
   {
      m_pEditor->onSampleSelectionUpdated( pKeyboard );
   }
}


SamplerEngine::Engine *PluginProcessor::samplerEngine() const
{
   return( m_pEngine );
}

