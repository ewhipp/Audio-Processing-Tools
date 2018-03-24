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
                                                    public ChangeBroadcaster,
                                                    public CompressorProcessor
{
public:
    //==============================================================================
    AmericanUniversityCompressorAudioProcessor();
    ~AmericanUniversityCompressorAudioProcessor();

    float currentGainFactor;
    float currentdB;
    float currentRMS;
    float thresholdRMS;
    float calculateOvershoot(float rmsAmp, float threshold);
    float calculateDesiredGain(float threshold, float ratio, float Overshoot);
    float calculateGainFactor(float desiredGain, float rmsAmp);
    float rmsAmp(int n, const float *buffer);
    float rms2dB(float rmsAmplitude);
    float calculateNumSamples(float*, int, int);
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
   // void parameterChanged(const String& parameter, float newValue) override;

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
private:
    
    AudioProcessorValueTreeState parameters;
    float lastOvershoot;
    float currentOvershoot;
    float blockTargetGainFactor;
    bool attackFlag;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessor)
};
