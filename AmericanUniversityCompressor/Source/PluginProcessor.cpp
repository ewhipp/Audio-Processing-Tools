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
    ratio(*this, nullptr)
#endif
{
    gainFactor =   1.0f;
    attackFlag = false;
    releaseFlag = true;
    timeSinceAttack = 0;
    timeSinceRelease = 0; // set this to 1 so that we initiali
    numberOfSamplesToApplyGain = 1;
    sampleRate = AmericanUniversityCompressorAudioProcessor::getSampleRate();

    
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
    
   addParameter(release =       new AudioParameterFloat ("release",
                                                   "Release",
                                                   0.0f,
                                                   5000.0f,
                                                   300.0f));
    
    addParameter(attack =       new AudioParameterFloat ("attack",
                                                   "Attack",
                                                   0.0f,
                                                   5000.0f,
                                                   300.0f));
    
    ratio.createAndAddParameter("ratio", "Ratio", TRANS ("Ratio"),
                                NormalisableRange<float>(0.1, 10.0, 0.01),
                                2,
                                [](float value) { return "1 : " + String (value, 1); },
                                [](const String& text) { return text.substring(3).getFloatValue(); }, false, true, false);
    
    ratio.state = ValueTree (Identifier ("AmericanUniversityCompressor"));
    // Change to integers 1 --> 100
    /* addParameter(ratio =        new AudioParameterFloat ("ratio",
     "Ratio",
     1.0f,
     1000.0f,
     1.0f));*/
    
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
float AmericanUniversityCompressorAudioProcessor::calculateNumSamples(AudioParameterFloat* slider,
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
        float currentRatio = *ratio.getRawParameterValue("ratio");
        const float* channelData = buffer.getReadPointer(channel);
        float tempThresh = *threshold;
        currentRMS =   rmsAmp(buffer.getNumSamples(), channelData);
        currentdB =    Decibels::gainToDecibels(currentRMS);
        thresholdRMS = Decibels::decibelsToGain(tempThresh);
        
        
        // We are over the threshold when we have previously been under
        if (currentRMS > thresholdRMS && !attackFlag && !releaseFlag)
        {
            startingGain = currentGain;
            numberOfSamplesToApplyGain = calculateNumSamples(attack, sampleRate, buffer.getNumSamples());
            attackFlag = true;
            timeSinceAttack = 0;
            currentOvershoot = (currentRMS - thresholdRMS);
            
            if (currentRatio == 0)
                currentRatio = 1;
            
            desiredGain  = (currentOvershoot / currentRatio) + thresholdRMS;
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
        else if (currentRMS > thresholdRMS && attackFlag && !releaseFlag)
        {
            timeSinceAttack += buffer.getNumSamples();
            float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
            if (rampProgress >= 1)
            {
                blockTargetGain = gainFactor;
                attackFlag = false;
                releaseFlag = true;
            }
            else
            {
                blockTargetGain = startingGain - (rampProgress * gainRange);
            }
        }
        
        // When we first go back under after previously attacking
        else if (currentRMS <= thresholdRMS && releaseFlag && !isReleasing)
        {
            startingGain = currentGain;
            numberOfSamplesToApplyGain = calculateNumSamples(release, sampleRate, buffer.getNumSamples());
            timeSinceRelease = 0; // time since release TODO change
            gainFactor = 1.0f;
            isReleasing = true;
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
        else if (currentRMS <= thresholdRMS && releaseFlag && isReleasing)
        {
            timeSinceRelease += buffer.getNumSamples();
            float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
            if (rampProgress >= 1)
            {
                blockTargetGain = gainFactor;
                releaseFlag = false;
                isReleasing = false;
            }
            else
                blockTargetGain = startingGain + (rampProgress * gainRange);
        }
        
        buffer.applyGainRamp(0, buffer.getNumSamples(), currentGain, blockTargetGain);
        currentGain = blockTargetGain;
        
        // Convert the gain to a dB value first, then apply as a dB value.
        float temporaryMakeupGain = *makeupGain;
        temporaryMakeupGain = Decibels::decibelsToGain(temporaryMakeupGain);
        buffer.applyGain(temporaryMakeupGain);
    }
}

//==============================================================================
bool AmericanUniversityCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmericanUniversityCompressorAudioProcessor::createEditor() 
{
    return new AmericanUniversityCompressorAudioProcessorEditor (*this, ratio);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    MemoryOutputStream (destData, true).writeFloat(*makeupGain);
    MemoryOutputStream (destData, true).writeFloat(*threshold);
    //MemoryOutputStream (destData, true).writeFloat(*ratio);
    MemoryOutputStream (destData, true).writeFloat(*release);
    MemoryOutputStream (destData, true).writeFloat(*attack);
}

void AmericanUniversityCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // estore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    *makeupGain = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *threshold = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    //*ratio = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *release = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    *attack = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmericanUniversityCompressorAudioProcessor();
}
