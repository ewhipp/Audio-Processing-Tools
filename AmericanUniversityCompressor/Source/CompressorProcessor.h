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
   
    void beginAttack(float numberOfSamplesToApplyGain, float blockTargetGain, int timeSinceAttack,
                     float gainFactor, float startingGainFactor)
    {
        if (numberOfSamplesToApplyGain == 0)
            blockTargetGain = gainFactor;
        else
        {
            float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
            float gainFactorRange = startingGainFactor - gainFactor;
            blockTargetGain = startingGainFactor - (rampProgress * gainFactorRange);
        }
    }
    
    void continueAttack(float timeSinceAttack, float numberOfSamplesToApplyGain, float blockTargetGain, float gainFactor, float startingGainFactor)
    {
        float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            blockTargetGain = gainFactor;
        else
            blockTargetGain = startingGainFactor - (rampProgress * gainFactor);
    }
    
    void beginRelease(float currentGain, float numberOfSamplesToApplyGain, float blockTargetGain, float timeSinceRelease)
    {
        float startingGainFactor = currentGain;
        timeSinceRelease = 0;
        float gainFactor = 1.0f;
        if (numberOfSamplesToApplyGain == 0)
            blockTargetGain = gainFactor;
        else
        {
            float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
            float gainFactorRange = gainFactor - startingGainFactor;
            blockTargetGain = startingGainFactor + (rampProgress * gainFactorRange);
        }
    }
    
    void continueRelease(float timeSinceRelease, float numberOfSamplesToApplyGain,
                         float gainFactorRange, float blockTargetGainFactor,
                         float startingGainFactor, float gainFactor)
    {
        float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            blockTargetGainFactor = gainFactor;
        else
            blockTargetGainFactor = startingGainFactor + (rampProgress * gainFactorRange);
    }
    
    void attackRampProgress(float timeSinceAttack, float numberOfSamplesToApplyGain,
                            float gainFactor, float blockTarget, float startingGainFactor,
                            float gainFactorRange)
    {
        float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            blockTarget = gainFactor;
        else
            blockTarget = startingGainFactor - (rampProgress * gainFactorRange);
    }
/*
    void releaseRampProgress (float timeSinceRelease, float numberOfSamplesToApplyGain,
                              float gainFactor, float blockTarget, float gainRightNow)
    {
        if (numberOfSamplesToApplyGain == 0)
            blockTarget = gainFactor;
        else
        {
            float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
            gainFactorRange = gainFactor - gainRightNow;
            blockTarget = gainFactor + (rampProgress * gainFactorRange);
        }
    }
    
    void initCurrentGain(float currentGain)
    {
        startingGainFactor = currentGain;
    }
    
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
    
    void setBlockSize(int m)
    {
        blockSize = m;
    }
    
    void setParentSampleRate (int n)
    {
        sampleRate = n;
    }
    
    int getBlockSize()
    {
        return blockSize;
    }
    
    int getParentSampleRate()
    {
        return sampleRate;
    }
    */
};

