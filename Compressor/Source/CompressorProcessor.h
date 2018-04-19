/*
  ==============================================================================

    CompressorProcessor.h
    Created: 4 Feb 2018 7:21:19pm
    Author:  Erik Whipp
 
    This file will help calculate the required preliminaries to effectively
    process the incoming audio.
  ==============================================================================
*/

#pragma once

class CompressorProcessor
{
public:
    CompressorProcessor(){}
    
    ~CompressorProcessor() {}
   
    // This function represents when our current samples are going over the threshold.
    float beginAttack(float startingGainFactor, float* ratioSlider, float* attackSlider,
                     float currentOvershoot, float thresholdRMS, float currentRMS)
    {
        float blockTargetGain;
        setStartingGainFactor (startingGainFactor);
        numberOfSamplesToApplyGain = calculateNumSamples (attackSlider, sampleRate, blockSize);
        setTimeSinceAttack (0);
        desiredGainFactor = calculateDesiredGain (currentOvershoot, thresholdRMS, *ratioSlider);
        gainFactor = calculateGainFactor (desiredGainFactor, currentRMS);
        timeSinceAttack += blockSize;
        
        if (numberOfSamplesToApplyGain == 0)
            return blockTargetGain = gainFactor;
        else
        {
            float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
            gainFactorRange = startingGainFactor - gainFactor;
            return blockTargetGain = startingGainFactor - (rampProgress * gainFactorRange);
        }
        
    }
    
    // This function represents when our overshoot has not been recalculated and we are still attacking.
    float continueAttack ()
    {
        float blockTargetGain;
        timeSinceAttack += blockSize;
        float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            return blockTargetGain = gainFactor;
        else
            return blockTargetGain = startingGainFactor - (rampProgress * gainFactor);
    }
    /*
     * This function represents when our compressor has gotten us below the threshold and we are getting back
     * to normal.
     */
    float beginRelease(float startingGainFactor, float* releaseSlider)
    {
        float blockTargetGain;
        setStartingGainFactor (startingGainFactor);
        setTimeSinceRelease (0);
        gainFactor = 1.0f;
        numberOfSamplesToApplyGain = calculateNumSamples (releaseSlider, sampleRate, blockSize);
        
        timeSinceRelease += blockSize;
        if (numberOfSamplesToApplyGain == 0)
            return blockTargetGain = gainFactor;
        else
        {
            float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
            gainFactorRange = gainFactor - startingGainFactor;
            return blockTargetGain = startingGainFactor + (rampProgress * gainFactorRange);
        }
    }
    
    // This function represents when we are continuing our release and have not had to recalculate parameters
    float continueRelease()
    {
        float blockTargetGainFactor;
        timeSinceRelease += blockSize;
        float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            return blockTargetGainFactor = gainFactor;
        else
            return blockTargetGainFactor = startingGainFactor + (rampProgress * gainFactorRange);
    }

    /* Residual calculations */
    float calculateOvershoot(float currentRMS, float currentThreshold)
    {
        currentOvershoot = currentRMS - currentThreshold;
        return currentOvershoot;
    }
    
    float calculateDesiredGain(float currentOvershoot, float currentThreshold,
                              float currentRatio)
    {
        desiredGainFactor = (currentOvershoot / currentRatio) + currentThreshold;
        return desiredGainFactor;
    }
    
    float calculateGainFactor(float desiredGain, float currentRMS)
    {
        gainFactor = desiredGain / currentRMS;
        return gainFactor;
    }
    
    // Calculate the number of samples the compressor must take to reach the desired gain factor.
    float calculateNumSamples (float* ratioSlider, int parentSampleRate, int blockSize)
    {
        float timeToWaste;
        float sliderValue = *ratioSlider / 1000;
        timeToWaste = sliderValue * parentSampleRate;
        
        if (blockSize > timeToWaste)
            timeToWaste = 0;
        return timeToWaste;
    }
    
    /* SETTERS */
    void setBlockSize(int m)
    {
        blockSize = m;
    }
    
    void setParentSampleRate (int n)
    {
        sampleRate = n;
    }
    
    void setTimeSinceRelease(int o)
    {
        timeSinceRelease = o;
    }
    
    void setTimeSinceAttack(int l)
    {
        timeSinceAttack = l;
    }
    
    void setStartingGainFactor(float m)
    {
        startingGainFactor = m;
    }
    
    /* GETTERS */
    int getBlockSize()
    {
        return blockSize;
    }
    
    int getParentSampleRate()
    {
        return sampleRate;
    }
    
    int getTimeSinceRelease()
    {
        return timeSinceRelease;
    }
    
    int getTimeSinceAttack()
    {
        return timeSinceAttack;
    }
    
    float getStartingGainFactor()
    {
        return startingGainFactor;
    }
    
private:
    float desiredGainFactor;
    float gainFactor;
    float numberOfSamplesToApplyGain;
    float startingGainFactor;
    float gainFactorRange;
    float currentOvershoot;
    
    int timeSinceRelease;
    int timeSinceAttack;
    int blockSize;
    int sampleRate;
};

