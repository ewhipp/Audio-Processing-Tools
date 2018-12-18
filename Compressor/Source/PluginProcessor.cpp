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
                       )
#endif
{
    attackFlag = false;
    lastOvershoot = -1;
    
    addParameter (attack = new AudioParameterFloat ("attack", "Attack",
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
                                                   }));
    
    addParameter (release = new AudioParameterFloat ("release", "Release",
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
                                                     }));
    
    addParameter (threshold = new AudioParameterFloat ("threshold", "Threshold",
                                                      NormalisableRange<float>(-100.f, 0.0f, 1.0f), -16.0f, translate("Threshold"),
                                                      AudioProcessorParameter::inputMeter,
                                                      [] (float value, int maximumStringLength)
                                                      { return String (value, 1) + "dB"; },
                                                      [] (const String& text)
                                                      { return text.substring(0, text.length() - 2).getFloatValue(); }));
    
    addParameter (makeupGain = new AudioParameterFloat ("makeUpGain", "Make-up Gain",
                                                        NormalisableRange<float>(0.0f, 12.0f, 1.0f), 0.0f,
                                                        translate("Make-up Gain"),
                                                        AudioProcessorParameter::inputMeter,
                                                        [] (float value, int maximumStringLength)
                                                            { return String (Decibels::gainToDecibels(value)) + " dB"; },
                                                        [] (const String& text)
                                                        { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }));

    addParameter (ratio = new AudioParameterFloat ("ratio", "Ratio",
                                                   NormalisableRange<float>(1.0f, 10.f, 1.0f), 2.0f,
                                                   translate("Ratio"),
                                                   AudioProcessorParameter::inputMeter,
                                                   [](float value, int maximumStringLength) { return "1:" + String (value, 1); },
                                                   [](const String& text) { return text.substring(3).getFloatValue();
                                                   }));
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
    
    
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        currentRMS =   ampToRMS (buffer.getNumSamples(), channelData);
        currentdB =    Decibels::gainToDecibels (currentRMS);
        thresholdRMS = Decibels::decibelsToGain (threshold->get());
        currentOvershoot = (currentRMS - thresholdRMS);
        
        if (currentRMS > thresholdRMS && currentOvershoot != lastOvershoot)
        {
            attackFlag = true;
            blockTargetGainFactor = compressor->beginAttack (currentGainFactor, ratio->get(), attack->get(),
                                                             currentOvershoot, thresholdRMS, currentRMS);
        }
        
        else if (currentRMS > thresholdRMS && currentOvershoot == lastOvershoot)
            blockTargetGainFactor = compressor->continueAttack();
        
        else if (currentRMS <= thresholdRMS && attackFlag)
        {
            attackFlag = false;
            blockTargetGainFactor = compressor->beginRelease(currentGainFactor, release->get());
        }
        
        else if (currentRMS <= thresholdRMS && !attackFlag)
            blockTargetGainFactor = compressor->continueRelease();
        
        buffer.applyGainRamp (0, buffer.getNumSamples(), currentGainFactor, blockTargetGainFactor);
        currentGainFactor = blockTargetGainFactor;
        
        // Convert the gain to a dB value first, then apply as a dB value.
        buffer.applyGain(Decibels::decibelsToGain(makeupGain->get()));
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
    std::unique_ptr<XmlElement> xml (new XmlElement ("Compressor"));
    xml->setAttribute ("attack", (double) *attack);
    xml->setAttribute ("release", (double) *release);
    xml->setAttribute ("threshold", (double) *threshold);
    xml->setAttribute ("makeupGain", (double) *makeupGain);
    xml->setAttribute ("ratio", (double) *ratio);
    copyXmlToBinary (*xml, destData);
}

void AmericanUniversityCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName ("Compressor"))
        {
            *attack =     xmlState->getDoubleAttribute("attack", 0.0f);
            *release =    xmlState->getDoubleAttribute("release", 300.f);
            *threshold =  xmlState->getDoubleAttribute("threshold", -6.0f);
            *makeupGain = xmlState->getDoubleAttribute("makeUpGain", 0.0f);
            *ratio =      xmlState->getDoubleAttribute("ratio", 2.0f);
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


