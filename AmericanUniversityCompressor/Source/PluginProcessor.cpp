/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
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
                       ),
parameters(*this, nullptr)
#endif
{
    attackFlag = false;
    CompressorProcessor::setTimeSinceRelease (0);
    CompressorProcessor::setTimeSinceRelease (0);
    lastOvershoot = -1;

    parameters.createAndAddParameter("attack", "Attack", TRANS("Attack"),
                                 NormalisableRange<float>(0.0f, 5000.0f, 0.001f), 0.0f,
                                 [] (float value)
                                 {
                                     if (value < 0.001f)  return String (value) + "µs";
                                     if (value >= 1000.0f)  return String (value) + "s";
                                     else  return String (value) + "ms";
                                 },
                                 [] (const String& text) // probably a crap way of doing this
                                 {
                                     int lengthNeeded = text.length() - 2;
                                     if (text.containsAnyOf("µs")) return text.substring(0, lengthNeeded).getFloatValue() * 1000;
                                     if (text.containsAnyOf("s")) return text.substring(0, lengthNeeded + 1).getFloatValue() / 1000;
                                     else
                                         return text.substring(0, lengthNeeded).getFloatValue(); // ms
                                 }
                                 );
    
    parameters.createAndAddParameter("release", "Release", TRANS("Release"),
                                 NormalisableRange<float>(0.0f, 5000.0f, 0.001f), 300.0f,
                                 [] (float value)
                                 {
                                     if (value < 0.001f)  return String (value) + "µs";
                                     if (value >= 1000.0f)  return String (value) + "s";
                                     else  return String (value) + "ms";
                                 },
                                 [] (const String& text) // probably a crap way of doing this
                                 {
                                     int lengthNeeded = text.length() - 2;
                                     if (text.containsAnyOf("µs")) return text.substring(0, lengthNeeded).getFloatValue() * 1000;
                                     if (text.containsAnyOf("s")) return text.substring(0, lengthNeeded + 1).getFloatValue() / 1000;
                                     else
                                         return text.substring(0, lengthNeeded).getFloatValue(); // ms
                                 }
                                 );
    
    parameters.createAndAddParameter("threshold", "Threshold", TRANS("Threshold"),
                                 NormalisableRange<float>(-100.0f, 0.0f, 1.0f), -16.0f,
                                 [] (float value)
                                     { return String (value, 1) + "dB"; },
                                 [] (const String& text)
                                     { return text.substring(0, text.length() - 2).getFloatValue(); });
    
    parameters.createAndAddParameter("makeUpGain", "Make-up Gain", TRANS("Make-up Gain"),
                                 NormalisableRange<float>(0.0f, 12.0f, 1.0f), 0.0f,
                                     [] (float value) { return String (Decibels::gainToDecibels(value)) + " dB"; },
                                     [] (const String& text) { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); },
                                     false, true, false);

    parameters.createAndAddParameter("ratio", "Ratio", String(),
                                NormalisableRange<float>(1.0f, 10.0, 1.0f),
                                2.0f,
                                [](float value) { return "1:" + String (value, 1); }, // return 1:n
                                [](const String& text) { return text.substring(3).getFloatValue(); }); // retrieve n
    
    parameters.state = ValueTree (Identifier ("AmericanUniversityCompressor"));
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
    return 1;
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
    currentGainFactor = 1.0f;
    CompressorProcessor::setParentSampleRate(sampleRate);
    CompressorProcessor::setBlockSize(samplesPerBlock);
}

void AmericanUniversityCompressorAudioProcessor::releaseResources()
{
    
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
        total += powf(buffer[i],2.0);
    
    total /= n;
    total = sqrt(total);
    return total;
}

/*
 * Attack and release start in milliseconds convert to seconds and then samples and round
 * divide the slider value by 1000 * by sample rate
 */
// Calculate time to wait for attack/release sliders for more info: see CompressorProcessor.h
float AmericanUniversityCompressorAudioProcessor::calculateNumSamples(float* slider,
                                                                      int n, int blockSize)
{
    float timeToWaste;
    float sliderValue = *slider;
    sliderValue = *slider / 1000;
    timeToWaste = sliderValue * n;
    if (blockSize > timeToWaste) { timeToWaste = 0;}
    return timeToWaste;
}

// Main function for audio processing
void AmericanUniversityCompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Get our parameter values.
    float* makeupGain = parameters.getRawParameterValue("makeUpGain");
    float* threshold = parameters.getRawParameterValue("threshold");
    float* attack = parameters.getRawParameterValue("attack");
    float* release = parameters.getRawParameterValue("release");
    float* ratio = parameters.getRawParameterValue("ratio");

    
    // Clear the buffer in order to reduce the chances of returning feedback
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is where we actually process the audio
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        currentRMS =   rmsAmp(buffer.getNumSamples(), channelData);
        currentdB =    Decibels::gainToDecibels(currentRMS);
        thresholdRMS = Decibels::decibelsToGain(*threshold);
        currentOvershoot = (currentRMS - thresholdRMS);

        if (currentRMS > thresholdRMS && currentOvershoot != lastOvershoot)
        {
            attackFlag = true;
            blockTargetGainFactor = CompressorProcessor::beginAttack (currentGainFactor, ratio, attack,
                                             currentOvershoot, thresholdRMS, currentRMS);
        }
        
        else if (currentRMS > thresholdRMS && currentOvershoot == lastOvershoot)
            blockTargetGainFactor = CompressorProcessor::continueAttack();
        
        else if (currentRMS <= thresholdRMS && attackFlag)
        {
            attackFlag = false;
            blockTargetGainFactor = CompressorProcessor::beginRelease(currentGainFactor, release);
        }
        else if (currentRMS <= thresholdRMS && !attackFlag)
            blockTargetGainFactor = CompressorProcessor::continueRelease();

        
        buffer.applyGainRamp(0, buffer.getNumSamples(), currentGainFactor, blockTargetGainFactor);
        currentGainFactor = blockTargetGainFactor;
        
        // Convert the gain to a dB value first, then apply as a dB value.
        buffer.applyGain(Decibels::decibelsToGain(*makeupGain));
        lastOvershoot = currentOvershoot;
        
        // Allow the user to visualize the compressed audio.
        visualizeBuffer.makeCopyOf(buffer);
    }
}

//==============================================================================
bool AmericanUniversityCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmericanUniversityCompressorAudioProcessor::createEditor() 
{
    return new AmericanUniversityCompressorAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AmericanUniversityCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // estore your parameters from this memory block,
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
    return new AmericanUniversityCompressorAudioProcessor();
}
