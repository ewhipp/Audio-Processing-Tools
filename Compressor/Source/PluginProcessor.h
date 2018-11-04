/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CompressorProcessor.h"



//==============================================================================
/**
*/
class AmericanUniversityCompressorAudioProcessor  : public AudioProcessor,
                                                    public ChangeBroadcaster
{
public:
    //==============================================================================
    AmericanUniversityCompressorAudioProcessor();
    ~AmericanUniversityCompressorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    float getCurrentdB();
    float getCurrentGainFactor();
    float getCurrentThresholdRMS();
    float getCurrentRMS();
    float getTargetGainFactor();
    
    AudioSampleBuffer getVisualBuffer();
    
    int getVisualBufferChannels();

    //==============================================================================

private:
    AudioProcessorValueTreeState parameters;
    
    float lastOvershoot;
    float currentOvershoot;
    float blockTargetGainFactor;
    float currentGainFactor;
    float currentRMS;
    float thresholdRMS;
    float currentdB;
    
    CompressorProcessor* compressor;
    
    bool attackFlag;
    
    AudioSampleBuffer visualizeBuffer;

    
    /**
     * Moving RMS function.
     *
     * @param n: number to finish window
     * @param buffer: current buffer amplitude amount.
     */
    float ampToRMS (int n, const float *buffer)
    {
        float total = 0.0;
        
        for (int i = 0; i < n; i++)
            total += powf(buffer[i], 2.0);
        
        total /= n;
        total = sqrt(total);
        return total;
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessor)
};
