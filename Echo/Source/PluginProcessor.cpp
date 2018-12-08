/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EchoAudioProcessor::EchoAudioProcessor()
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
parameters (*this, nullptr)
{
    parameters.createAndAddParameter("delay_time", "Delay", translate("Delay"),
                                     NormalisableRange<float>(0.0f, 5000.0f, 0.001f), 0.0f,
                                     [] (float value)
                                     {
                                         if (value < 000.1f)  return String (value) + "µs";
                                         if (value >= 1000.f) return String (value) + "s";
                                         else return String (value) + "ms";
                                     },
                                     [] (const String& text)
                                     {
                                         if (text.containsAnyOf("µs"))
                                             return text.substring(0, text.length() - 2).getFloatValue() * 1000;
                                         else if (text.containsAnyOf("s"))
                                             return text.substring(0, text.length() - 1).getFloatValue() / 1000;
                                         else
                                             return text.substring(0, text.length() - 2).getFloatValue();
                                     }, false, true, false);
    
    parameters.createAndAddParameter("delay_feedback", "Feedback", translate("Feedback"),
                                     NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.7,
                                     [] (float value)
                                        { return String (value * 100.f) + "%"; },
                                     [] (const String& text)
                                        { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                     false, true, false);
    
    parameters.createAndAddParameter("delay_toggle", "Toggle", translate("Toggle"),
                                     NormalisableRange<float>(0.0f, 1.0f, 1.0f), 0.0f,
                                     [] (float value) { return value < 0.5 ? "Off" : "On"; },
                                     [] (const String& text)
                                     {
                                         if (text == "Off")     return 0.0f;
                                         else if (text == "On") return 1.0f;
                                         else                   return 0.0f;
                                     }, false, true, false);
    
    parameters.state = ValueTree (Identifier ("Echo"));
}

EchoAudioProcessor::~EchoAudioProcessor()
{
}

//==============================================================================
const String EchoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EchoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EchoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EchoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EchoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EchoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EchoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EchoAudioProcessor::setCurrentProgram (int index)
{
}

const String EchoAudioProcessor::getProgramName (int index)
{
    return {};
}

void EchoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EchoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EchoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EchoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EchoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool EchoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EchoAudioProcessor::createEditor()
{
    return new EchoAudioProcessorEditor (*this);
}

//==============================================================================
void EchoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    stateOfPlugin.reset(parameters.state.createXml());
    copyXmlToBinary(*stateOfPlugin, destData);
}

void EchoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    stateOfPlugin.reset(getXmlFromBinary(data, sizeInBytes));
    if (stateOfPlugin != nullptr)
        if (stateOfPlugin->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*stateOfPlugin);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EchoAudioProcessor();
}
