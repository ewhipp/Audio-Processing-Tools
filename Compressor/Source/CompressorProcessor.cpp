/*
  ==============================================================================

    CompressorProcessor.cpp
    Created: 4 Aug 2018 8:17:19pm
    Author:  Erik Whipp

  ==============================================================================
*/

#include "CompressorProcessor.h"

CompressorProcessor::CompressorProcessor(float parentSampleRate, int userDeterminedBlockSize)
{
    sampleRate = parentSampleRate;
    blockSize = userDeterminedBlockSize;
}

CompressorProcessor::~CompressorProcessor() { }

float CompressorProcessor::beginAttack (float startingGainFactor, float* ratioSlider, float* attackSlider,
                                 float currentOvershoot, float thresholdRMS, float currentRMS)
{
    setStartingGainFactor (startingGainFactor);
    setTimeSinceAttack (0);

    numberOfSamplesToApplyGain = calculateNumSamples (attackSlider, sampleRate, blockSize);
    desiredGainFactor = calculateDesiredGain (currentOvershoot, thresholdRMS, *ratioSlider);
    gainFactor = calculateGainFactor (desiredGainFactor, currentRMS);
    
    timeSinceAttack += blockSize;
    
    if (numberOfSamplesToApplyGain == 0)
        return gainFactor;
    else
        return startingGainFactor - ((timeSinceAttack / numberOfSamplesToApplyGain) * (startingGainFactor - gainFactor));
}

float CompressorProcessor::continueAttack()
{
    timeSinceAttack += blockSize;
    
    if ((timeSinceAttack / numberOfSamplesToApplyGain) >= 1)
        return gainFactor;
    else
        return startingGainFactor - ((timeSinceAttack / numberOfSamplesToApplyGain) * gainFactor);
}

float CompressorProcessor::beginRelease(float startingGainFactor, float* releaseSlider)
{
    setStartingGainFactor (startingGainFactor);
    setTimeSinceRelease (0);
    
    gainFactor = 1.0f;
    numberOfSamplesToApplyGain = calculateNumSamples (releaseSlider, sampleRate, blockSize);
    timeSinceRelease += blockSize;
    
    if (numberOfSamplesToApplyGain == 0)
        return gainFactor;
    else
        return startingGainFactor + ((timeSinceRelease / numberOfSamplesToApplyGain) * (gainFactor - startingGainFactor));
}

float CompressorProcessor::continueRelease()
{
    timeSinceRelease += blockSize;
    
    if ((timeSinceRelease / numberOfSamplesToApplyGain) >= 1)
        return gainFactor;
    else
        return startingGainFactor + ((timeSinceRelease / numberOfSamplesToApplyGain) * gainFactorRange);
}
