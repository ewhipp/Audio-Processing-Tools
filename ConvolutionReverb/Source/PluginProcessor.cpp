/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ConvolutionReverbAudioProcessor::ConvolutionReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                     #endif
                       ),
#endif
Thread ("Background Thread"), parameters(*this, nullptr), reverbProcessor(512, 44100)
{
    // Pre-delay
    parameters.createAndAddParameter ("pre_delay", "Pre-delay", TRANS ("Pre-delay"),
                                      NormalisableRange <float> (0.0f, 5000.0f, 0.001f), 0.0f,
                                      [] ( float value )
                                      {
                                          if ( value < 0.001f ) return String ( value ) + "µs";
                                          if ( value >= 1000.0f ) return String ( value ) + "s";
                                          else return String ( value ) + "ms";
                                      },
                                      [] (const String& text)
                                      {
                                         int lengthNeeded = text.length() - 2;
                                         if (text.containsAnyOf("µs"))
                                             return text.substring(0, lengthNeeded).getFloatValue() * 1000;
                                         if (text.containsAnyOf("s"))
                                             return text.substring(0, lengthNeeded + 1).getFloatValue() / 1000;
                                         else
                                             return text.substring(0, lengthNeeded).getFloatValue(); // ms
                                      }, false, true, false);
    // Dry
    parameters.createAndAddParameter ("dry", "Dry", TRANS ("Dry"),
                                      NormalisableRange <float> (0.0f, 1.0f, 0.01f), 0.5f,
                                      [] ( float value )
                                      { return String ( value * 100.0f ) + "%"; },
                                      [] ( const String& text )
                                      { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                        false, true, false );
    // Wet
    parameters.createAndAddParameter ("wet", "Wet", TRANS ("Wet"),
                                      NormalisableRange <float> (0.0f, 1.0f, 0.01f), 0.5f,
                                      [] ( float value )
                                      { return String ( value * 100.0f ) + "%"; },
                                      [] ( const String& text )
                                      { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                      false, true, false );
    
    // Freq
    // What to put here?
    
    // Register reading formats
    formatManager.registerBasicFormats();
    parameters.state = ValueTree (Identifier ("ConvolutionReverb"));
    startThread();
}

ConvolutionReverbAudioProcessor::~ConvolutionReverbAudioProcessor()
{
    currentBuffer = nullptr;
    stopThread (4000);
}

//==============================================================================
const String ConvolutionReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//==============================================================================
// Threading Begin
void ConvolutionReverbAudioProcessor::run()
{
    while (! threadShouldExit())
    {
        // Open file system dialog and then check if we need to free any of the
        // thread buffers
        openFromFileSystem();
        checkForBuffersToFree();
        
        // 100 ms time frame
        wait (1);
    }
}

/*
 * Threading cont'd
 * Audio thread should only be referencing one 500ms of file data at a time
 * This is useful because we don't want to have any audio data glitches and
 * thus we have file reading on a separate thread.
 */
void ConvolutionReverbAudioProcessor::checkForBuffersToFree()
{
    for (auto i = buffers.size(); --i >= 0;)
    {
        FileBuffer::Ptr buffer (buffers.getUnchecked (i));
        
        // We have used up a 500ms time frame so remove that last buffer.
        if (buffer->getReferenceCount() == 2)
            buffers.remove(i);
    }
}

// Actually loading the file into memory to be played by the thread buffers
void ConvolutionReverbAudioProcessor::openFromFileSystem()
{
    String filePath;
    filePath.swapWith (chosenPath);
    
    if (filePath.isNotEmpty())
    {
        File file (filePath);
        std::unique_ptr<AudioFormatReader> fileReader (formatManager.createReaderFor(file));
        
        if (fileReader.get() != nullptr)
        {
            // get the duration of the audio file
            auto duration = fileReader->lengthInSamples / fileReader->sampleRate;
        
            // 60 second sound file takes 2116,8000 bytes of memory
            if (duration <= 60)
            {
                FileBuffer::Ptr newBuffer = new FileBuffer (file.getFileName(), fileReader->numChannels, (int) fileReader->lengthInSamples);
                
                // This is where the audio file is placed into our own array buffer
                fileReader->read(newBuffer->getAudioSampleBuffer(), 0, (int) fileReader->lengthInSamples, 0, true, true);
                
                // Add to the thread buffers in the processor
                currentBuffer = newBuffer;
                buffers.add (newBuffer);
            }
            else
            {
                // Error handling
                // TODO: create a dialog window or decrease length of file?
                DBG ("File is too long, please try another file.");
            }
        }
    }
}

// Threading Completed
//==============================================================================


bool ConvolutionReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ConvolutionReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ConvolutionReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ConvolutionReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ConvolutionReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int ConvolutionReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ConvolutionReverbAudioProcessor::setCurrentProgram (int index)
{
}

const String ConvolutionReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void ConvolutionReverbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ConvolutionReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void ConvolutionReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ConvolutionReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ConvolutionReverbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    float* convOutput;
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < 1; ++channel)
    {
        for(int j = 0; j < getBlockSize(); j++)
            reverbProcessor.setCurrentAudio (buffer.getSample(channel, j));
        
        reverbProcessor.computeRealTimeFFT();
        reverbProcessor.accumulateComplexValues();
        convOutput = reverbProcessor.outputConvolution();
        
        buffer.copyFrom (channel, 0, convOutput, buffer.getNumSamples());
    }
}

// Retrieve the FFT values of the IR wav file that was loaded in.
void ConvolutionReverbAudioProcessor::computeIRFFT()
{
    reverbProcessor.getImpulseResponseFileFFT (currentBuffer);
}

//==============================================================================
bool ConvolutionReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ConvolutionReverbAudioProcessor::createEditor()
{
    return new ConvolutionReverbAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void ConvolutionReverbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ConvolutionReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ConvolutionReverbAudioProcessor();
}
