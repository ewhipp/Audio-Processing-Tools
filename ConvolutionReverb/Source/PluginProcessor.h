/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FileBuffer.h"
#include "ReverbProcessor.h"


//==============================================================================
class ConvolutionReverbAudioProcessor  : public AudioProcessor,
                                         public ChangeBroadcaster,
                                         public Thread
{
public:
    float dry;
    float wet;
    float preDelay;
    
    //==============================================================================
    ConvolutionReverbAudioProcessor();
    ~ConvolutionReverbAudioProcessor();

    //==============================================================================
    
    // Thread safe reading of the convolution file
    ReferenceCountedArray<FileBuffer> buffers;
    FileBuffer::Ptr currentBuffer;
    void openFromFileSystem();
    String chosenPath;
    void computeIRFFT();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    void run() override;
    void checkForBuffersToFree();

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
private:
    AudioProcessorValueTreeState parameters;            // Values of our current slider parameters
    AudioFormatManager formatManager;                   // Read in audio formats
    ReverbProcessor reverbProcessor;                    // The Reverb class that performs the Convolution
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbAudioProcessor)
};


