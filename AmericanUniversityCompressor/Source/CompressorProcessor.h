/*
  ==============================================================================

    CompressorProcessor.h
    Created: 4 Feb 2018 7:21:19pm
    Author:  Erik Whipp
 
    This file will help calculate the required preliminaries to effectively
    process the incoming audio. The following are the calculations:
 
    calculateOvershoot = RMS - Threshold
    calculateDesiredGain = (Overshoot / ratio) + threshold
    calculateGainFactor = desiredGain / RMS
  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "chrono"
#include "ctime"

class CompressorProcessor
{
public:
    CompressorProcessor() {}
    ~CompressorProcessor() {}
    
    float calculateOvershoot(float currentRMS, float currentThreshold)
    {
        overshoot = currentRMS - currentThreshold;
        return overshoot;
    }
    
    float calculateDesiredGain(float currentOvershoot, float currentThreshold,
                              float currentRatio)
    {
        desiredGain = (currentOvershoot / currentRatio) + currentThreshold;
        return desiredGain;
    }
    
    float calculateGainFactor(float desiredGain, float currentRMS)
    {
        gainFactor = desiredGain / currentRMS;
        return gainFactor;
    }
    
    // Ms --> Sample function
    // Take in sample rate
    // take in current slider value
    // 1 = 44.1k samples, 0.5 = 22.05k samples 0.0 = no wait
    float calculateMilliseconds(AudioParameterFloat* sliderValue,
                               AudioSampleBuffer& targetSamples)
    {
        float timeToWaste;
        int systemSampleRate = targetSamples.getNumSamples();
        // Multiply slider value by the sample rate in order to see how many
        // samples it must take?
        timeToWaste = *sliderValue * systemSampleRate;
        return timeToWaste;
    }
    
private:
    float gainFactor;
    float desiredGain;
    float overshoot;
};

