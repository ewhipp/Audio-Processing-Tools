/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

 
    For 20 - 2 - 18
    Attack/Release to millis -- Check!
    When attack/release are 0  -- Check!
    Makeup Gain as an additive to normal gain  Check!
    Test signals for attack/release
    Display the currentGain etc on a label  Check!
 
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
                       ),
    parameters(*this, nullptr)
#endif
{
    gainFactor =   1.0f;
    attackFlag = false;
    releaseFlag = true;
    timeSinceAttack = 0;
    timeSinceRelease = 0; // set this to 1 so that we initiali
    numberOfSamplesToApplyGain = 1;
    sampleRate = AmericanUniversityCompressorAudioProcessor::getSampleRate();

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
                                NormalisableRange<float>(0.0f, 10.0, 1.0f),
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
    currentGain = 1.0f;
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
    // Convert to seconds
    sliderValue = *slider / 1000;
    // Multiply by sample rate
    // How many samples we should waste.
    timeToWaste = sliderValue * n;
    // If blocksize is greater than the samples we should waste, we should just go straight to the value
    if (blockSize > timeToWaste) { timeToWaste = 0;}
    return timeToWaste;
}

// Main function for audio processing
void AmericanUniversityCompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    
    
    // Clear the buffer in order to reduce the chances of returning feedback
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is where we actually process the audio
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // get parameter values
        float* makeupGain = parameters.getRawParameterValue("makeUpGain");
        float* threshold = parameters.getRawParameterValue("threshold");
        float* attack = parameters.getRawParameterValue("attack");
        float* release = parameters.getRawParameterValue("release");
        float* ratio = parameters.getRawParameterValue("ratio");
        const float* channelData = buffer.getReadPointer(channel);
        float tempThresh = *threshold;
        currentRMS =   rmsAmp(buffer.getNumSamples(), channelData);
        currentdB =    Decibels::gainToDecibels(currentRMS);
        thresholdRMS = Decibels::decibelsToGain(tempThresh);
        
        
        // We are over the threshold when we have previously been under
        if (currentRMS > thresholdRMS && !attackFlag)
        {
            startingGain = currentGain;
            numberOfSamplesToApplyGain = calculateNumSamples(attack, sampleRate, buffer.getNumSamples());
            attackFlag = true;
            releaseFlag = false;
            timeSinceAttack = 0;
            currentOvershoot = (currentRMS - thresholdRMS);
            
            if (*ratio == 0.0f)
                *ratio = 1.0f;
            
            desiredGain  = (currentOvershoot / *ratio) + thresholdRMS;
            gainFactor = desiredGain / currentRMS;
            timeSinceAttack += buffer.getNumSamples();
            
            if (numberOfSamplesToApplyGain == 0)
                blockTargetGain = gainFactor;
            else
            {
                float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
                gainRange = startingGain - gainFactor;
                blockTargetGain = startingGain - (rampProgress * gainRange);
            }
        }
        // When we are already attacking and we are above thresh
        else if (currentRMS > thresholdRMS && attackFlag)
        {
            timeSinceAttack += buffer.getNumSamples();
            float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
            if (rampProgress >= 1)
            {
                blockTargetGain = gainFactor;
                attackFlag = false;
            }
            else
            {
                blockTargetGain = startingGain - (rampProgress * gainRange);
            }
        }
        
        // When we first go back under after previously attacking
        else if (currentRMS <= thresholdRMS && attackFlag)
        {
            attackFlag = false;
            releaseFlag = true;
            startingGain = currentGain;
            numberOfSamplesToApplyGain = calculateNumSamples(release, sampleRate, buffer.getNumSamples());
            timeSinceRelease = 0; // time since release TODO change
            gainFactor = 1.0f;
            timeSinceRelease += buffer.getNumSamples();
            if (numberOfSamplesToApplyGain == 0)
                blockTargetGain = gainFactor;
            else
            {
                float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
                gainRange = startingGain + gainFactor ;
                blockTargetGain = startingGain + (rampProgress * gainRange);
            }
        }
        
        // When we are under the threshold and releasing. This is also our starting condition if we're below thresh
        else if (currentRMS <= thresholdRMS && releaseFlag)
        {
            timeSinceRelease += buffer.getNumSamples();
            float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
            if (rampProgress >= 1)
            {
                blockTargetGain = gainFactor;
            }
            else
                blockTargetGain = startingGain + (rampProgress * gainRange);
        }
        
        buffer.applyGainRamp(0, buffer.getNumSamples(), currentGain, blockTargetGain);
        currentGain = blockTargetGain;
        
        // Convert the gain to a dB value first, then apply as a dB value.
        buffer.applyGain(Decibels::decibelsToGain(*makeupGain));
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
    
}

void AmericanUniversityCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // estore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmericanUniversityCompressorAudioProcessor();
}
