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
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this, nullptr)
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
                                         if (text.containsAnyOf("µs")) return text.substring(0, lengthNeeded).getFloatValue() * 1000;
                                         if (text.containsAnyOf("s")) return text.substring(0, lengthNeeded + 1).getFloatValue() / 1000;
                                         else
                                             return text.substring(0, lengthNeeded).getFloatValue(); // ms
                                      }, false, true, false);
    
    // Size
    parameters.createAndAddParameter ("size", "Size", TRANS ("Size"),
                                      NormalisableRange <float> (0.0f, 1.0f, 0.01f), 0.5f,
                                      [] ( float value )
                                      { return String ( value * 100.0f ) + "%"; },
                                      [] ( const String& text )
                                      { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                      false, true, false );
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
    // Width
    parameters.createAndAddParameter ("width", "Width", TRANS ("Width"),
                                      NormalisableRange <float> (0.0f, 1.0f, 0.01f), 0.5f,
                                      [] ( float value )
                                      { return String ( value ) + "%"; },
                                      [] ( const String& text )
                                      { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                      false, true, false );
    // Gain
    parameters.createAndAddParameter("gain", "Gain", TRANS("Gain"),
                                     NormalisableRange<float>(0.0f, 12.0f, 1.0f), 0.0f,
                                     [] (float value) { return String (Decibels::gainToDecibels(value)) + " dB"; },
                                     [] (const String& text) { return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); },
                                     false, true, false);
    
    // Damping
    parameters.createAndAddParameter ("damping", "Damping", TRANS ("Damping"),
                                      NormalisableRange <float> (0.0f, 1.0f, 0.01f), 0.5f,
                                      [] ( float value )
                                      { return String ( value * 100.0f ) + "%"; },
                                      [] ( const String& text )
                                      { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                      false, true, false );
    
    // Freeze mode
    parameters.createAndAddParameter ("freeze", "Freeze", TRANS ("Freeze"),
                                      NormalisableRange <float> (0.0f, 1.0f, 0.01f), 0.5f,
                                      [] ( float value )
                                      {  return String ( value * 100.0f ) + "%"; },
                                      [] ( const String& text )
                                      { return text.trimCharactersAtEnd("%").getFloatValue(); },
                                      false, true, false );
    // Freq
    // What to put here?
    
    
    parameters.state = ValueTree (Identifier ("ConvolutionReverb"));
}

ConvolutionReverbAudioProcessor::~ConvolutionReverbAudioProcessor()
{
}

//==============================================================================
const String ConvolutionReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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
    reverbState.reset();
    reverbParams.dryLevel = 0.2f;
    reverbParams.wetLevel = 0.0f;
    reverbParams.roomSize = 0.0f;
    reverbParams.damping = 0.0f;
    reverbParams.freezeMode = 0.4;
    reverbState.setSampleRate (sampleRate);
    reverbState.setParameters (reverbParams);
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
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // float*  preDelay = parameters.getRawParameterValue("pre_delay");
    float*  gain = parameters.getRawParameterValue("gain");
    float*  width = parameters.getRawParameterValue("width");
    float*  dry = parameters.getRawParameterValue("dry");
    float*  wet = parameters.getRawParameterValue("wet");
    float*  size = parameters.getRawParameterValue("size");
    float* damping = parameters.getRawParameterValue("damping");
    float* freeze = parameters.getRawParameterValue("freeze");

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

   
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
       // auto* channelData = buffer.getWritePointer (channel);
        
        // Reset params
        
        reverbParams.dryLevel = *dry;
        reverbParams.wetLevel = *wet;
        reverbParams.roomSize = *size;
        reverbParams.damping = *damping;
        reverbParams.freezeMode = *freeze;
        reverbParams.width = *width;
        reverbState.setParameters (reverbParams);

        updateParams();
        
        // If mono
        if (totalNumInputChannels == 1)
            reverbState.processMono (buffer.getWritePointer(0), buffer.getNumSamples());
        
        // If Stereo
        else if (totalNumInputChannels == 2)
            reverbState.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
        
        // Apply the gain
        buffer.applyGain(Decibels::decibelsToGain(*gain));
    }
}

void ConvolutionReverbAudioProcessor::updateParams()
{
    
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
