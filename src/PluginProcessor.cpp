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
   m_ofs = 0;
   m_note = -1;

   for( size_t i = 0; i < 16; i++ )
   {
      m_Parts.push_back( new SamplerEngine::Part( i ) );
   }
}


PluginProcessor::~PluginProcessor()
{
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      delete m_Parts[i];
   }
   m_Parts.clear();
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
   return( m_Parts[m_pEditor->currentPart()]->samples() );
}


const std::list<SamplerEngine::Sample *> &PluginProcessor::constSamples() const
{
   return( m_Parts[m_pEditor->currentPart()]->samples() );
}


bool PluginProcessor::midiNoteIsPlaying( int midiNote ) const
{
   return( m_Voices.count( midiNote ) > 0 );
}


std::set<int> PluginProcessor::allPlayingMidiNotes() const
{
   std::set<int> result;
   for( auto i = m_Voices.begin(); i != m_Voices.end(); i++ )
   {
      result.insert( i->first );
   }

   return( result );
}


bool PluginProcessor::isPlaying( const SamplerEngine::Sample *pSample ) const
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


void PluginProcessor::handleNoteOn( MidiKeyboardState *pSource, int midiChannel, int midiNoteNumber, float velocity )
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
   int vel = (int)( 127 * velocity );

   std::list<SamplerEngine::Sample *> s = getSamplesByMidiNoteAndVelocity( (size_t)( midiChannel - 1 ), midiNoteNumber, vel );
   for( SamplerEngine::Sample *pSample : s )
   {
      SamplerEngine::Voice *pVoice = new SamplerEngine::Voice( pSample, midiNoteNumber, vel );
      m_Voices.insert( std::pair{ midiNoteNumber, pVoice } );
   }

   m_note = midiNoteNumber;
}


std::list<SamplerEngine::Sample *> PluginProcessor::getSamplesByMidiNoteAndVelocity( size_t part, int note, int vel ) const
{
   std::list<SamplerEngine::Sample *> result;

   for( SamplerEngine::Sample *pSample : m_Parts[part]->samples() )
   {
      if( pSample->matchesMidiNote( note ) && pSample->matchesVelocity( vel ) )
      {
         result.push_back( pSample );
      }
   }

   return( result );
}


void PluginProcessor::handleNoteOff( MidiKeyboardState */*pSource*/, int /*midiChannel*/, int midiNoteNumber, float /*velocity*/ )
{
   if( m_Voices.count( midiNoteNumber ) > 0 )
   {
      auto v = m_Voices.equal_range( midiNoteNumber );
      for( auto voice = v.first; voice != v.second; voice++ )
      {
         SamplerEngine::Voice *pVoice = voice->second;
         pVoice->noteOff();
      }
   }

   m_note = -1;
}


void PluginProcessor::stopVoice( const SamplerEngine::Voice *pVoice )
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
   for (const juce::MidiMessageMetadata metadata : midiMessages)
   {
      const juce::MidiMessage msg = metadata.getMessage();
      if( msg.isNoteOn() )
      {
         int note = msg.getNoteNumber();
         m_note = note;
         noteOn( msg.getChannel(), msg.getNoteNumber(), msg.getVelocity() / 127.0f );
      } else
      if( msg.isNoteOff() )
      {
         int note = msg.getNoteNumber();
         if( m_note == note )
         {
            m_note = -1;
         }
         noteOff( msg.getChannel(), msg.getNoteNumber(), msg.getVelocity() / 127.0f );
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
   for( int i = 0; i < getBusCount( false ); i++ )
   {
      const Bus *pBus = getBus( false, i );
      if( pBus->isEnabled() )
      {
         AudioBuffer b = pBus->getBusBuffer( buffer );
         for( int channel = 0; channel < b.getNumChannels(); channel++ )
         {
            float *pChannelData = b.getWritePointer( channel );
            for( int j = 0; j < b.getNumSamples(); j++ )
            {
               pChannelData[j] = 0.0;
            }
         }
      }
   }
/*   for( int channel = 0; channel < totalNumOutputChannels; ++channel )
   {
      auto *pChannelData = buffer.getWritePointer( channel );
      for( auto j = 0; j < m_samplesPerBlock; j++ )
      {
         pChannelData[j] = 0.0;
      }
   }*/

   std::set<SamplerEngine::Voice *> stoppedVoices;
   for( auto k = m_Voices.begin(); k != m_Voices.end(); k++ )
   {
      SamplerEngine::Voice *pVoice = k->second;
      int busNum = pVoice->sample()->getOutputBus();
      if( busNum < 0 )
         busNum = 0;

      if( !outputBusReady( buffer, busNum ) )
      {
         stoppedVoices.insert( pVoice );
      } else
      {
         AudioBuffer buf = getBus( false, busNum )->getBusBuffer( buffer );
         float *pLeft = buf.getWritePointer( 0 );
         float *pRight = buf.getWritePointer( 1 );

         if( !pVoice->process( pLeft, pRight, buf.getNumSamples(), m_sampleRate ) )
         {
            stoppedVoices.insert( pVoice );
         }
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
   juce::XmlElement vt( "overvoltage" );

   juce::XmlElement *peParts = new juce::XmlElement( "parts" );
   for( size_t i = 0; i < m_Parts.size(); i++ )
   {
      juce::XmlElement *pePart = m_Parts[i]->getStateInformation();
      peParts->addChildElement( pePart );
   }

   vt.addChildElement( peParts );

   copyXmlToBinary( vt, destData );
   std::string s = vt.toString().toStdString();
   printf("\n");
}


void PluginProcessor::setStateInformation( const void* data, int sizeInBytes )
{
   // You should use this method to restore your parameters from this memory block,
   // whose contents will have been created by the getStateInformation() call.
   std::unique_ptr<juce::XmlElement> xmlState( getXmlFromBinary( data, sizeInBytes ) );
   if( xmlState.get() )
   {
      if( xmlState.get()->getTagName() == "overvoltage" )
      {
         juce::XmlElement *peOvervoltage = xmlState.get();
         for( int i = 0; peOvervoltage->getChildElement( i ); i++ )
         {
            if( peOvervoltage->getChildElement( i )->getTagName() == "parts" )
            {
               juce::XmlElement *peParts = peOvervoltage->getChildElement( i );
               for( int nPart = 0; peParts->getChildElement( nPart ); nPart++ )
               {
                  if( peParts->getChildElement( nPart )->getTagName() == "part" )
                  {
                     juce::XmlElement *pePart = peParts->getChildElement( nPart );
                     SamplerEngine::Part *pPart = SamplerEngine::Part::fromXml( pePart );
                     if( pPart )
                     {
                        if( m_Parts[pPart->getPartNum()] )
                        {
                           delete m_Parts[pPart->getPartNum()];
                        }
                        m_Parts[pPart->getPartNum()] = pPart;
                     }
                  }
               }
            }
         }
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
   deleteSample( part, pSample );
}


void PluginProcessor::deleteSample( size_t part, SamplerEngine::Sample *pSample )
{
   std::vector<SamplerEngine::Voice *> voicesToStop;
   for( std::pair<int, SamplerEngine::Voice *> sv : m_Voices )
   {
      if( sv.second->sample() == pSample )
      {
         voicesToStop.push_back( sv.second );
      }
   }

   for( SamplerEngine::Voice *pVoice : voicesToStop )
   {
      stopVoice( pVoice );
   }

   m_Parts[part]->deleteSample( pSample );
}
