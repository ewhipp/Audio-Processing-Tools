/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class AmericanUniversityCompressorAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AmericanUniversityCompressorAudioProcessor();
    ~AmericanUniversityCompressorAudioProcessor();

    float currentGain;
    float currentdB;
    float currentRMS;
    float calculateOvershoot(float rmsAmp, float threshold);
    float calculateDesiredGain(float threshold, float ratio, float Overshoot);
    float calculateGainFactor(float desiredGain, float rmsAmp);
    float rmsAmp(int n, const float *buffer);
    float rms2dB(float rmsAmplitude);
    float calculateNumSamples(AudioParameterFloat*, int, int);
    
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
    
private:
    AudioParameterFloat* makeupGain;
    AudioParameterFloat* threshold;
    // AudioParameterFloat* ratio;
    AudioProcessorValueTreeState ratio;
    
    AudioParameterFloat* attack;
    AudioParameterFloat* release;
        
    float blockTargetGain;
    float currentOvershoot;
    float desiredGain;
    float gainFactor;
    float thresholdRMS;
    float numberOfSamplesToApplyGain;
    float startingGain;
    float gainRange;
    
    double sampleRate;
    
    int timeSinceRelease;
    int timeSinceAttack;
    bool attackFlag;
    bool releaseFlag;
    bool isReleasing;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessor)
};
