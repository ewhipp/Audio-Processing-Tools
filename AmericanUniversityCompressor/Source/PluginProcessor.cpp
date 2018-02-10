/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "CompressorProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AmericanUniversityCompressorAudioProcessor::AmericanUniversityCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(makeupGain =   new AudioParameterFloat ("makeupGain",
                                                       "Make-up Gain",
                                                       0.0f,
                                                       12.0f,
                                                       0.0f));
    
    addParameter(threshold =    new AudioParameterFloat ("threshold",
                                                      "Threshold",
                                                      -100.0f,
                                                      0.0f,
                                                      -16.0f));
    
    // Change to integers 1 --> 100
    addParameter(ratio =        new AudioParameterFloat ("ratio",
                                                  "Ratio",
                                                  1.0f,
                                                  100.0f,
                                                  2.0f));
    
   addParameter(release =       new AudioParameterFloat ("release",
                                                   "Release",
                                                   0.0f,
                                                   1.0000f,
                                                   0.250f));
    
    addParameter(attack =       new AudioParameterFloat ("attack",
                                                   "Attack",
                                                   0.0f,
                                                   1.0000f,
                                                   0.250f));
    
}

AmericanUniversityCompressorAudioProcessor::~AmericanUniversityCompressorAudioProcessor()
{
}

//==============================================================================
const String AmericanUniversityCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmericanUniversityCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmericanUniversityCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AmericanUniversityCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AmericanUniversityCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmericanUniversityCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmericanUniversityCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmericanUniversityCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String AmericanUniversityCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmericanUniversityCompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    rampedGain = *makeupGain;
}

void AmericanUniversityCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmericanUniversityCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

float AmericanUniversityCompressorAudioProcessor::rmsAmp(int n, const float *buffer)
{
    float total;
    total = 0.0;
    for(int i = 0; i < n; i++)
        total += powf(buffer[i],2.0);// * buffer[i];
    
    total /= n;
    total = sqrt(total);
    return total;
}


// Main function for audio processing
void AmericanUniversityCompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // How JUCE explains to ramp the gain;    
    const float currentGain = *makeupGain;

    if (currentGain == rampedGain) { buffer.applyGain (currentGain); }
    else {
        buffer.applyGainRamp (0, buffer.getNumSamples(), rampedGain, currentGain);
        rampedGain = currentGain;
    }

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);

        float tempThresh = *threshold;
        currentRMS = rmsAmp(buffer.getNumSamples(), channelData);
        currentdB = Decibels::gainToDecibels(currentRMS);
        thresholdRMS = Decibels::decibelsToGain(tempThresh);
        gainFactor = 1.0f;
        
        // For more information: see CompressorProcessor.h
        //
        // TODO: conditional logic for these moments?
        if ( currentRMS > thresholdRMS)
        {
            currentOvershoot = (currentRMS - thresholdRMS);
            desiredGain = (currentOvershoot / *ratio) + thresholdRMS;
            gainFactor = desiredGain / currentRMS;
            buffer.applyGain(gainFactor);
            buffer.applyGain(*makeupGain);
        }
    }
}

//==============================================================================
bool AmericanUniversityCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmericanUniversityCompressorAudioProcessor::createEditor()
{
    return new AmericanUniversityCompressorAudioProcessorEditor (*this);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    MemoryOutputStream (destData, true).writeFloat(*makeupGain);
    MemoryOutputStream (destData, true).writeFloat(*threshold);
    MemoryOutputStream (destData, true).writeFloat(*ratio);
    MemoryOutputStream (destData, true).writeFloat(*release);
    MemoryOutputStream (destData, true).writeFloat(*attack);
}

void AmericanUniversityCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // estore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    *makeupGain = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *threshold = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *ratio = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *release = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *attack = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmericanUniversityCompressorAudioProcessor();
}


// Rms to dB
/*
float AmericanUniversityCompressorAudioProcessor::rms2dB(float rmsAmplitude)
{
    float dbAmplitude;
    dbAmplitude = 20.0f * (log10f(rmsAmplitude)/0.00001f) - 100.0f;// -6.0206
    return dbAmplitude;
}
*/
