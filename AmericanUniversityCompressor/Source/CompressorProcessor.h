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
    CompressorProcessor(int SampleRate, int BlockSize)
    {
        sampleRate = SampleRate;
        blockSize = BlockSize;
    }
    
    ~CompressorProcessor() {}
   
    void beginAttack(float currentGain, float numberOfSamplesToApplyGain, float currentRMS,
                      float thresholdRMS, bool attackFlag, float* ratio, float blockTargetGain)
    {
        initCurrentGain (currentGain);
        attackFlag = true;
        timeSinceAttack = 0;
        overshoot = calculateOvershoot(currentRMS, thresholdRMS);
        desiredGain = calculateDesiredGain(overshoot, thresholdRMS, *ratio);
        gainFactor = calculateGainFactor(desiredGain, currentRMS);
        
        timeSinceAttack += blockSize;
        attackRampProgress(timeSinceAttack, numberOfSamplesToApplyGain, gainFactor,
                           blockTargetGain, startingGain);
        
    }
    
    void continueAttack(float timeSinceAttack, float numberOfSamplesToApplyGain)
    {
        timeSinceAttack += blockSize;
        float rampProgress = timeSinceAttack / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            blockTargetGain = gainFactor;
        else
            blockTargetGain = startingGain - (rampProgress * gainFactor);
    }
    
    void beginRelease(float currentGain, float numberOfSamplesToApplyGain, float blockTargetGain)
    {
        attackFlag = false;
        initCurrentGain (currentGain);
        timeSinceRelease = 0;
        gainFactor = 1.0f;
        timeSinceRelease += blockSize;
        
        releaseRampProgress(timeSinceRelease, numberOfSamplesToApplyGain,
                            gainFactor, blockTargetGain, startingGain);
    }
    
    void continueRelease(float timeSinceRelease, float blockSize, float numberOfSamplesToApplyGain,
                         float gainFactor, float gainFactorRange, float blockTargetGainFactor)
    {
        timeSinceRelease += blockSize;
        float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
        if (rampProgress >= 1)
            blockTargetGainFactor = gainFactor;
        else
            blockTargetGainFactor = startingGain + (rampProgress * gainFactorRange);
    }
    
    void attackRampProgress(float timeSinceAttack, float numberOfSamplesToApplyGain,
                            float gainFactor, float blockTarget, float gainRightNow)
    {
        if (numberOfSamplesToApplyGain == 0)
            blockTargetGain = gainFactor;
        else
        {
            float currentProgress = timeSinceAttack / numberOfSamplesToApplyGain;
            gainRange = gainRightNow - gainFactor;
            blockTargetGain = startingGain - (currentProgress * gainRange);
        }
    }
    
    void releaseRampProgress (float timeSinceRelease, float numberOfSamplesToApplyGain,
                              float gainFactor, float blockTarget, float gainRightNow)
    {
        if (numberOfSamplesToApplyGain == 0)
            blockTarget = gainFactor;
        else
        {
            float rampProgress = timeSinceRelease / numberOfSamplesToApplyGain;
            gainRange = gainFactor - gainRightNow;
            blockTarget = gainFactor + (rampProgress * gainRange);
        }
    }
    
    void initCurrentGain(float currentGain)
    {
        startingGain = currentGain;
    }
    
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
    float timeSinceAttack;
    float timeSinceRelease;
    
    float gainFactor;
    float desiredGain;
    float overshoot;
    float startingGain;
    float gainRange;
    float blockTargetGain;
    
    float sampleRate;
    float blockSize;
    
    bool attackFlag;
};

