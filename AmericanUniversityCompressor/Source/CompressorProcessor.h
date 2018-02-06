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
private:
    float gainFactor;
    float desiredGain;
    float overshoot;
};

