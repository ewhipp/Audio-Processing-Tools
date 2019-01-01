/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

typedef CompressorAudioProcessor::CompressorParameters compParams;

String CompressorAudioProcessor::getParameterId (size_t index)
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

float* CompressorAudioProcessor::getParameterValue (String parameterId)
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
    
    std::vector <std::unique_ptr <AudioProcessorParameterGroup> > params;
    
    // Begin Float parameters
    auto attack
    = std::make_unique <AudioParameterFloat>
                                             (CompressorAudioProcessor::getParameterId (compParams::ATTACK), "Attack",
                                                    NormalisableRange<float>(0.0f, 5000.0f, 0.001f), 0.0f, translate("Attack"),
                                                    AudioProcessorParameter::inputMeter,
                                                    [] (float value, int maximumStringLength)
                                                    {
                                                        if (value < 0.001f)
                                                            return String (value) + "µs";
                                                        if (value >= 1000.0f)
                                                            return String (value / 1000.0f) + "s";
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
                                                  (CompressorAudioProcessor::getParameterId (compParams::RELEASE), "Release",
                                                     NormalisableRange<float>(0.0f, 5000.f, 0.001f), 300.f, translate("Release"),
                                                     AudioProcessorParameter::inputMeter,
                                                     [] (float value, int maximumStringLength)
                                                     {
                                                         if (value < 0.001f)
                                                             return String (value) + "µs";
                                                         if (value >= 1000.0f)
                                                             return String (value / 1000.0f) + "s";
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
                                                  (CompressorAudioProcessor::getParameterId (compParams::THRESHOLD), "Threshold",
                                                       NormalisableRange<float>(-100.f, 0.0f, 1.0f), -16.0f, translate("Threshold"),
                                                       AudioProcessorParameter::inputMeter,
                                                       [] (float value, int maximumStringLength)
                                                       { return String (value, 1) + "dB"; },
                                                       [] (const String& text)
                                                       { return text.substring(0, text.length() - 2).getFloatValue(); });
    
    auto makeupGain
        = std::make_unique <AudioParameterFloat>
                                                  (CompressorAudioProcessor::getParameterId (compParams::MAKEUPGAIN), "Make-up Gain",
                                                        NormalisableRange<float>(0.0f, 12.0f, 1.0f), 0.0f,
                                                        translate("Make-up Gain"),
                                                        AudioProcessorParameter::inputMeter,
                                                        [] (float value, int maximumStringLength)
                                                        { return String (Decibels::gainToDecibels(value)) + " dB"; },
                                                        [] (const String& text)
                                                        { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); });
    
    auto ratio = std::make_unique <AudioParameterFloat>
                                                   (CompressorAudioProcessor::getParameterId (compParams::RATIO), "Ratio",
                                                   NormalisableRange<float>(1.0f, 10.f, 1.0f), 2.0f,
                                                   translate("Ratio"),
                                                   AudioProcessorParameter::inputMeter,
                                                   [](float value, int maximumStringLength) { return "1:" + String (value, 0); },
                                                   [](const String& text) { return text.substring(3).getFloatValue();
                                                });
    
    // Begin Bool parameters
    auto knee = std::make_unique <AudioParameterBool>
                                                  (CompressorAudioProcessor::getParameterId (compParams::KNEE), "Knee", false);
    
    auto floatParams = std::make_unique <AudioProcessorParameterGroup> ("float-values", "Floats", "|",
                                                                        std::move (attack),
                                                                        std::move (release),
                                                                        std::move (threshold),
                                                                        std::move (makeupGain),
                                                                        std::move (ratio));
    params.push_back (std::move (floatParams));
    
    auto boolParams = std::make_unique <AudioProcessorParameterGroup> ("bool-values", "Bools", "|",
                                                                       std::move (knee));
    
    params.push_back (std::move (boolParams));
    
    return { params.begin(), params.end() };
}

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
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
    m_lastOvershoot = -1;
    
    state.addParameterListener (getParameterId (compParams::ATTACK), this);
    state.addParameterListener (getParameterId (compParams::RELEASE), this);
    state.addParameterListener (getParameterId (compParams::THRESHOLD), this);
    state.addParameterListener (getParameterId (compParams::MAKEUPGAIN), this);
    state.addParameterListener (getParameterId (compParams::RATIO), this);
    state.addParameterListener (getParameterId (compParams::KNEE), this);
    
    state.state = ValueTree (JucePlugin_Name);
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
}

//==============================================================================
const String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_currentGainFactor = 1.0f;
    m_compressor = std::make_unique <CompressorProcessor> (sampleRate, samplesPerBlock);
}

void CompressorAudioProcessor::releaseResources() { }

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void CompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    float* attack     = getParameterValue (getParameterId (compParams::ATTACK));
    float* release    = getParameterValue (getParameterId (compParams::RELEASE));
    float* threshold  = getParameterValue (getParameterId (compParams::THRESHOLD));
    float* ratio      = getParameterValue (getParameterId (compParams::RATIO));
    float* makeupGain = getParameterValue (getParameterId (compParams::MAKEUPGAIN));
    float*  knee       = getParameterValue (getParameterId (compParams::KNEE));
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        m_currentRMS =   ampToRMS (buffer.getNumSamples(), channelData);
        m_currentdB =    Decibels::gainToDecibels (m_currentRMS);
        m_thresholdRMS = Decibels::decibelsToGain (*threshold);
        m_currentOvershoot = (m_currentRMS - m_thresholdRMS);
                
        if (m_currentRMS > m_thresholdRMS && m_currentOvershoot != m_lastOvershoot)
        {
            attackFlag = true;
            m_blockTargetGainFactor = m_compressor->beginAttack (m_currentGainFactor, *ratio, *attack,
                                                                 m_currentOvershoot, m_thresholdRMS, m_currentRMS);
        }
        
        else if (m_currentRMS > m_thresholdRMS && m_currentOvershoot == m_lastOvershoot)
            m_blockTargetGainFactor = m_compressor->continueAttack();
        
        else if (m_currentRMS <= m_thresholdRMS && attackFlag)
        {
            attackFlag = false;
            m_blockTargetGainFactor = m_compressor->beginRelease (m_currentGainFactor, *release);
        }
        
        else if (m_currentRMS <= m_thresholdRMS && !attackFlag)
            m_blockTargetGainFactor = m_compressor->continueRelease();
        
        if (*knee)
            m_compressor->engageHardKnee (*threshold, *ratio, channelData, 1)
        
        buffer.applyGainRamp (0, buffer.getNumSamples(), m_currentGainFactor, m_blockTargetGainFactor);
        m_currentGainFactor = m_blockTargetGainFactor;
        
        // Convert the gain to a dB value first, then apply as a dB value.
        buffer.applyGain (Decibels::decibelsToGain (*makeupGain));
        
        m_lastOvershoot = m_currentOvershoot;
        visualizeBuffer.makeCopyOf(buffer);
    }
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; 
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor() 
{
    return new CompressorAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    MemoryOutputStream stream (destData, false);
    state.state.writeToStream (stream);
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ValueTree tree = ValueTree::readFromData (data, size_t (sizeInBytes));
    
    if (tree.isValid())
        state.state = tree;
}

AudioProcessorValueTreeState& CompressorAudioProcessor::getPluginState()
{
    return state;
}

void CompressorAudioProcessor::parameterChanged(const String& parameter, float newValue)
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
    else if (parameter == getParameterId (compParams::KNEE))
    {
        auto param = getParameterValue (getParameterId (compParams::KNEE));
        param = &newValue;
    }
}

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}

float CompressorAudioProcessor::getCurrentdB()              { return m_currentdB; }
float CompressorAudioProcessor::getCurrentGainFactor()      { return m_currentGainFactor; }
float CompressorAudioProcessor::getCurrentThresholdRMS()    { return m_thresholdRMS; }
float CompressorAudioProcessor::getCurrentRMS()             { return m_currentRMS; }
float CompressorAudioProcessor::getTargetGainFactor()       { return m_blockTargetGainFactor; }
float CompressorAudioProcessor::getCurrentOvershoot()       { return m_currentOvershoot; }

AudioSampleBuffer CompressorAudioProcessor::getVisualBuffer() { return visualizeBuffer; }
int CompressorAudioProcessor::getVisualBufferChannels()       { return visualizeBuffer.getNumChannels(); }

