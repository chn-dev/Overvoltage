#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
   : AudioProcessor( BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
   .withInput( "Input", juce::AudioChannelSet::stereo(), true )
#endif
   .withOutput( "Output", juce::AudioChannelSet::stereo(), true )
#endif
   ), m_pEditor( 0 )
{
   m_ofs = 0;
   m_note = -1;
}


AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}


//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
   return JucePlugin_Name;
}


bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
   return( true );
#else
   return( false );
#endif
}


bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
   return( true );
#else
   return( false );
#endif
}


bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
   return( true );
#else
   return( false );
#endif
}


double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
   return( 0.0 );
}


int AudioPluginAudioProcessor::getNumPrograms()
{
   return( 1 ); // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}


int AudioPluginAudioProcessor::getCurrentProgram()
{
   return( 0 );
}


void AudioPluginAudioProcessor::setCurrentProgram( int index )
{
   juce::ignoreUnused( index );
}


const juce::String AudioPluginAudioProcessor::getProgramName( int index )
{
   juce::ignoreUnused( index );
   return {};
}


void AudioPluginAudioProcessor::changeProgramName( int index, const juce::String &newName )
{
   juce::ignoreUnused( index, newName );
}


//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay( double sampleRate, int samplesPerBlock )
{
   m_sampleRate = sampleRate;
   m_samplesPerBlock = samplesPerBlock;
}


void AudioPluginAudioProcessor::releaseResources()
{
   // When playback stops, you can use this as an opportunity to free up any
   // spare memory, etc.
}


bool AudioPluginAudioProcessor::isBusesLayoutSupported( const BusesLayout& layouts ) const
{
#if JucePlugin_IsMidiEffect
   juce::ignoreUnused( layouts );
   return( true );
#else
   // This is the place where you check if the layout is supported.
   // In this template code we only support mono or stereo.
   // Some plugin hosts, such as certain GarageBand versions, will only
   // load plugins that support stereo bus layouts.
   if( layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
       layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo() )
      return( false );

   // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
   if( layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet() )
      return( false );
#endif

   return( true );
#endif
}


std::list<Sample *> &AudioPluginAudioProcessor::samples()
{
   return( m_Samples );
}


const std::list<Sample *> &AudioPluginAudioProcessor::constSamples() const
{
   return( m_Samples );
}


bool AudioPluginAudioProcessor::midiNoteIsPlaying( int midiNote ) const
{
   return( m_Voices.count( midiNote ) > 0 );
}


std::set<int> AudioPluginAudioProcessor::allPlayingMidiNotes() const
{
   std::set<int> result;
   for( auto i = m_Voices.begin(); i != m_Voices.end(); i++ )
   {
      result.insert( i->first );
   }

   return( result );
}


bool AudioPluginAudioProcessor::isPlaying( const Sample *pSample ) const
{
   std::set<int> midiNotes = allPlayingMidiNotes();
   for( auto v : m_Voices )
   {
      if( v.second->sample() == pSample )
      {
         return( true );
      }
   }

   return( false );
}


void AudioPluginAudioProcessor::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, velocity );
/* MONO mode
   if( midiNoteIsPlaying( midiNoteNumber ) )
   {
      auto v = m_Voices.equal_range( midiNoteNumber );
      for( auto voice = v.first; voice != v.second; voice++ )
      {
         Voice *pVoice = voice->second;
         delete pVoice;
      }
      m_Voices.erase( midiNoteNumber );
   }
*/
   int vel = 127 * velocity;

   std::list<Sample *> s = getSamplesByMidiNoteAndVelocity( midiNoteNumber, vel );
   for( Sample *pSample : s )
   {
      Voice *pVoice = new Voice( pSample, midiNoteNumber, vel );
      m_Voices.insert( std::pair{ midiNoteNumber, pVoice } );
   }

   m_note = midiNoteNumber;
}


std::list<Sample *> AudioPluginAudioProcessor::getSamplesByMidiNoteAndVelocity( int note, int vel ) const
{
   std::list<Sample *> result;

   for( Sample *pSample : m_Samples )
   {
      if( pSample->matchesMidiNote( note ) && pSample->matchesVelocity( vel ) )
      {
         result.push_back( pSample );
      }
   }

   return( result );
}


void AudioPluginAudioProcessor::handleNoteOff( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
{
   juce::ignoreUnused( pSource, midiChannel, midiNoteNumber, velocity );

   if( m_Voices.count( midiNoteNumber ) > 0 )
   {
      auto v = m_Voices.equal_range( midiNoteNumber );
      for( auto voice = v.first; voice != v.second; voice++ )
      {
         Voice *pVoice = voice->second;
         pVoice->noteOff();
      }
   }

   m_note = -1;
}


void AudioPluginAudioProcessor::stopVoice( const Voice *pVoice )
{
   for( auto v = m_Voices.begin(); v != m_Voices.end(); v++ )
   {
      if( v->second == pVoice )
      {
         m_Voices.erase( v );
         delete pVoice;
         return;
      }
   }
}


void AudioPluginAudioProcessor::processBlock( juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages )
{
   for (const auto metadata : midiMessages)
   {
      const auto msg = metadata.getMessage();
      if( msg.isNoteOn() )
      {
         int note = msg.getNoteNumber();
         m_note = note;
         noteOn( 1, msg.getNoteNumber(), msg.getVelocity() / 127.0f );
      } else
      if( msg.isNoteOff() )
      {
         int note = msg.getNoteNumber();
         if( m_note == note )
         {
            m_note = -1;
         }
         noteOff( 1, msg.getNoteNumber(), msg.getVelocity() / 127.0f );
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
   for( int channel = 0; channel < totalNumOutputChannels; ++channel )
   {
      auto *pChannelData = buffer.getWritePointer( channel );
      for( auto j = 0; j < m_samplesPerBlock; j++ )
      {
         pChannelData[j] = 0.0;
      }
   }

   float *pLeft = buffer.getWritePointer( 0 );
   float *pRight = buffer.getWritePointer( 1 );

   std::set<Voice *> stoppedVoices;
   for( auto k = m_Voices.begin(); k != m_Voices.end(); k++ )
   {
      Voice *pVoice = k->second;
      if( !pVoice->process( pLeft, pRight, m_samplesPerBlock, m_sampleRate ) )
      {
         stoppedVoices.insert( pVoice );
      }
   }

   for( auto v : stoppedVoices )
   {
      stopVoice( v );
   }

   if( stoppedVoices.size() > 0 )
   {
      if( m_pEditor )
      {
         const MessageManagerLock mmLock;
         m_pEditor->repaint();
      }
   }
/*
    if( m_note >= 0 )
    {
       double f = 440.0 * pow( 2.0, (double)( m_note - 69 ) / 12.0 );
       for( int channel = 0; channel < totalNumOutputChannels; ++channel )
       {
           auto *pChannelData = buffer.getWritePointer( channel );

           for( auto j = 0; j < m_samplesPerBlock; j++ )
           {
              pChannelData[j] = sinf( 2.0f * 3.1415f * ( ( f / m_sampleRate ) * (float)( m_ofs + j ) ) );
           }

       }

       m_ofs += m_samplesPerBlock;
    }
*/
}


//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
   return( true ); // (change this to false if you choose to not supply an editor)
}


juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
   m_pEditor = new AudioPluginAudioProcessorEditor( *this );
   return( m_pEditor );
}


//==============================================================================
void AudioPluginAudioProcessor::getStateInformation( juce::MemoryBlock& destData )
{
   // You should use this method to store your parameters in the memory block.
   // You could do that either as raw data, or use the XML or ValueTree classes
   // as intermediaries to make it easy to save and load complex data.
   juce::ignoreUnused (destData);
}


void AudioPluginAudioProcessor::setStateInformation( const void* data, int sizeInBytes )
{
   // You should use this method to restore your parameters from this memory block,
   // whose contents will have been created by the getStateInformation() call.
   juce::ignoreUnused (data, sizeInBytes);
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
   return( new AudioPluginAudioProcessor() );
}


void AudioPluginAudioProcessor::onDeleteSample( Sample *pSample )
{
   deleteSample( pSample );
}


void AudioPluginAudioProcessor::deleteSample( Sample *pSample )
{
   std::vector<Voice *> voicesToStop;
   for( std::pair<int, Voice *> sv : m_Voices )
   {
      if( sv.second->sample() == pSample )
      {
         voicesToStop.push_back( sv.second );
      }
   }

   for( Voice *pVoice : voicesToStop )
   {
      stopVoice( pVoice );
   }

   if( std::find( m_Samples.begin(), m_Samples.end(), pSample ) != m_Samples.end() )
   {
      m_Samples.remove( pSample );
      delete pSample;
   }
}
