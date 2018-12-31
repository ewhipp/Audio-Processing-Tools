/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

typedef AmericanUniversityCompressorAudioProcessor::CompressorParameters compParams;

String AmericanUniversityCompressorAudioProcessor::getParameterId (size_t index)
{
    switch (index)
    {
        case 0: return "attack";
        case 1: return "release";
        case 2: return "threshold";
        case 3: return "makeupGain";
        case 4: return "ratio";
        case 5: return "knee";
        default: break;
    }
    
    return "Unknown";
}

float* AmericanUniversityCompressorAudioProcessor::getParameterValue (String parameterId)
{
    if (parameterId == getParameterId (compParams::ATTACK))
        { return state.getRawParameterValue (getParameterId (compParams::ATTACK)); }
    
    else if (parameterId == getParameterId (compParams::RELEASE))
        { return state.getRawParameterValue (getParameterId (compParams::RELEASE)); }
    
    else if (parameterId == getParameterId (compParams::THRESHOLD))
        { return state.getRawParameterValue (getParameterId (compParams::THRESHOLD)); }
    
    else if (parameterId == getParameterId (compParams::MAKEUPGAIN))
        { return state.getRawParameterValue (getParameterId (compParams::MAKEUPGAIN)); }
    
    else if (parameterId == getParameterId (compParams::RATIO))
        { return state.getRawParameterValue (getParameterId (compParams::RATIO)); }
    
    else if (parameterId == getParameterId (compParams::KNEE))
        { return state.getRawParameterValue (getParameterId (compParams::KNEE)); }
    
    return nullptr;
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    // This vector holds every group of parameters.. if we want to add more than simple compressor functionality
    // We don't need AudioProcessorParameterGroup until this functionality is required
    // std::vector <std::unique_ptr <AudioProcessorParameterGroup> > params;
    
    std::vector <std::unique_ptr <AudioParameterFloat> > params;
    
    // TODO: clean
    auto attack
    = std::make_unique <AudioParameterFloat>
                                             (AmericanUniversityCompressorAudioProcessor::getParameterId (compParams::ATTACK), "Attack",
                                                    NormalisableRange<float>(0.0f, 5000.0f, 0.001f), 0.0f, translate("Attack"),
                                                    AudioProcessorParameter::inputMeter,
                                                    [] (float value, int maximumStringLength)
                                                    {
                                                        if (value < 0.001f)
                                                            return String (value) + "µs";
                                                        else if (value >= 1000.0f)
                                                            return String (value) + "s";
                                                        else
                                                            return String (value) + "ms";
                                                    },
                                                    [] (const String& text)
                                                    {
                                                        if (text.containsAnyOf("µs"))
                                                            return text.substring(0, text.length() - 2).getFloatValue() * 1000;
                                                        else if (text.containsAnyOf("s"))
                                                            return text.substring(0, text.length() - 1).getFloatValue() / 1000;
                                                        else
                                                            return text.substring(0, text.length() - 2).getFloatValue(); // ms
                                                    });
    
    auto release
        = std::make_unique <AudioParameterFloat>
                                                  (AmericanUniversityCompressorAudioProcessor::getParameterId (compParams::RELEASE), "Release",
                                                     NormalisableRange<float>(0.0f, 5000.f, 0.001f), 300.f, translate("Release"),
                                                     AudioProcessorParameter::inputMeter,
                                                     [] (float value, int maximumStringLength)
                                                     {
                                                         if (value < 0.001f)
                                                             return String (value) + "µs";
                                                         if (value >= 1000.0f)
                                                             return String (value) + "s";
                                                         else
                                                             return String (value) + "ms";
                                                     },
                                                     [] (const String& text)
                                                     {
                                                         if (text.containsAnyOf("µs"))
                                                             return text.substring(0, text.length() - 2).getFloatValue() * 1000;
                                                         else if (text.containsAnyOf("s"))
                                                             return text.substring(0, text.length() - 1).getFloatValue() / 1000;
                                                         else
                                                             return text.substring(0, text.length() - 2).getFloatValue();
                                                     });
    
    auto threshold
        = std::make_unique <AudioParameterFloat>
                                                  (AmericanUniversityCompressorAudioProcessor::getParameterId (compParams::THRESHOLD), "Threshold",
                                                       NormalisableRange<float>(-100.f, 0.0f, 1.0f), -16.0f, translate("Threshold"),
                                                       AudioProcessorParameter::inputMeter,
                                                       [] (float value, int maximumStringLength)
                                                       { return String (value, 1) + "dB"; },
                                                       [] (const String& text)
                                                       { return text.substring(0, text.length() - 2).getFloatValue(); });
    
    auto makeupGain
        = std::make_unique <AudioParameterFloat>
                                                  (AmericanUniversityCompressorAudioProcessor::getParameterId (compParams::MAKEUPGAIN), "Make-up Gain",
                                                        NormalisableRange<float>(0.0f, 12.0f, 1.0f), 0.0f,
                                                        translate("Make-up Gain"),
                                                        AudioProcessorParameter::inputMeter,
                                                        [] (float value, int maximumStringLength)
                                                        { return String (Decibels::gainToDecibels(value)) + " dB"; },
                                                        [] (const String& text)
                                                        { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); });
    
    auto ratio = std::make_unique <AudioParameterFloat>
                                                   (AmericanUniversityCompressorAudioProcessor::getParameterId (compParams::RATIO), "Ratio",
                                                   NormalisableRange<float>(1.0f, 10.f, 1.0f), 2.0f,
                                                   translate("Ratio"),
                                                   AudioProcessorParameter::inputMeter,
                                                   [](float value, int maximumStringLength) { return "1:" + String (value, 1); },
                                                   [](const String& text) { return text.substring(3).getFloatValue();
                                                   });
    
    // TODO : Clean
    params.push_back (std::move (attack));
    params.push_back (std::move (release));
    params.push_back (std::move (threshold));
    params.push_back (std::move (makeupGain));
    params.push_back (std::move (ratio));
    
    return { params.begin(), params.end() };
}

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
#endif
state (*this, &undo, "PARAMS", createParameterLayout())
{
    attackFlag = false;
    lastOvershoot = -1;
    
    state.addParameterListener (getParameterId (compParams::ATTACK), this);
    state.addParameterListener (getParameterId (compParams::RELEASE), this);
    state.addParameterListener (getParameterId (compParams::THRESHOLD), this);
    state.addParameterListener (getParameterId (compParams::MAKEUPGAIN), this);
    state.addParameterListener (getParameterId (compParams::RATIO), this);
    // Add knee
    // state.addParameterListener (getParameterId (compParams::KNEE), this);
    
    state.state = ValueTree (JucePlugin_Name);

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
    compressor.reset (new CompressorProcessor(sampleRate, samplesPerBlock));
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

void AmericanUniversityCompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto attack     = getParameterValue (getParameterId (compParams::ATTACK));
    auto release    = getParameterValue (getParameterId (compParams::RELEASE));
    auto threshold  = getParameterValue (getParameterId (compParams::THRESHOLD));
    auto ratio      = getParameterValue (getParameterId (compParams::RATIO));
    auto makeupGain = getParameterValue (getParameterId (compParams::MAKEUPGAIN));
    
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        currentRMS =   ampToRMS (buffer.getNumSamples(), channelData);
        currentdB =    Decibels::gainToDecibels (currentRMS);
        thresholdRMS = Decibels::decibelsToGain (*threshold);
        currentOvershoot = (currentRMS - thresholdRMS);
        
        if (currentRMS > thresholdRMS && currentOvershoot != lastOvershoot)
        {
            attackFlag = true;
            blockTargetGainFactor = compressor->beginAttack (currentGainFactor, *ratio, *attack,
                                                             currentOvershoot, thresholdRMS, currentRMS);
        }
        
        else if (currentRMS > thresholdRMS && currentOvershoot == lastOvershoot)
            blockTargetGainFactor = compressor->continueAttack();
        
        else if (currentRMS <= thresholdRMS && attackFlag)
        {
            attackFlag = false;
            blockTargetGainFactor = compressor->beginRelease (currentGainFactor, *release);
        }
        
        else if (currentRMS <= thresholdRMS && !attackFlag)
            blockTargetGainFactor = compressor->continueRelease();
        
        buffer.applyGainRamp (0, buffer.getNumSamples(), currentGainFactor, blockTargetGainFactor);
        currentGainFactor = blockTargetGainFactor;
        
        // Convert the gain to a dB value first, then apply as a dB value.
        buffer.applyGain (Decibels::decibelsToGain (*makeupGain));
        
        lastOvershoot = currentOvershoot;
        visualizeBuffer.makeCopyOf(buffer);
    }
}

//==============================================================================
bool AmericanUniversityCompressorAudioProcessor::hasEditor() const
{
    return true; 
}

AudioProcessorEditor* AmericanUniversityCompressorAudioProcessor::createEditor() 
{
    return new AmericanUniversityCompressorAudioProcessorEditor (*this);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    MemoryOutputStream stream (destData, false);
    state.state.writeToStream (stream);
}

void AmericanUniversityCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ValueTree tree = ValueTree::readFromData (data, size_t (sizeInBytes));
    if (tree.isValid())
        state.state = tree;
}

AudioProcessorValueTreeState& AmericanUniversityCompressorAudioProcessor::getPluginState()
{
    return state;
}

void AmericanUniversityCompressorAudioProcessor::parameterChanged(const String& parameter, float newValue)
{
    if (parameter == getParameterId (compParams::ATTACK))
    {
        auto param = getParameterValue (getParameterId (compParams::ATTACK));
        param = &newValue;
    }
    else if (parameter == getParameterId (compParams::RELEASE))
    {
        auto param = getParameterValue (getParameterId (compParams::RELEASE));
        param = &newValue;
    }
    else if (parameter == getParameterId (compParams::THRESHOLD))
    {
        auto param = getParameterValue (getParameterId (compParams::THRESHOLD));
        param = &newValue;
    }
    else if (parameter == getParameterId (compParams::MAKEUPGAIN))
    {
        auto param = getParameterValue (getParameterId (compParams::MAKEUPGAIN));
        param = &newValue;
    }
    else if (parameter == getParameterId (compParams::RATIO))
    {
        auto param = getParameterValue (getParameterId (compParams::RATIO));
        param = &newValue;
    }
}

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmericanUniversityCompressorAudioProcessor();
}

float AmericanUniversityCompressorAudioProcessor::getCurrentdB() { return currentdB; }
float AmericanUniversityCompressorAudioProcessor::getCurrentGainFactor() { return currentGainFactor; }
float AmericanUniversityCompressorAudioProcessor::getCurrentThresholdRMS() { return thresholdRMS; }
float AmericanUniversityCompressorAudioProcessor::getCurrentRMS() { return currentRMS; }
float AmericanUniversityCompressorAudioProcessor::getTargetGainFactor() { return blockTargetGainFactor; }

AudioSampleBuffer AmericanUniversityCompressorAudioProcessor::getVisualBuffer() { return visualizeBuffer; }
int AmericanUniversityCompressorAudioProcessor::getVisualBufferChannels() { return visualizeBuffer.getNumChannels(); }

