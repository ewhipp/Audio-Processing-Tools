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
    CompressorProcessor() {}
    ~CompressorProcessor() {}
    
    
    /*
     * Planning for refactoring the compressor:
     * This is what we need --
     *      A solution for long expanses of code in the process audio block.. encapsulation.
     * -- REQUIREMENTS --
     * 1) When the attack goes over
     *      a) Take note of the currentGain
     *      b) calculate number of samples to apply gain
     *      c) Engage the attack flag
     *      d) Calculate overshoot
     *      e) Ratio handling (Can go anywhere)
     *      f) Calculate desiredGain
     *      g) Calculate gainFactor
     *      h) Add onto the timeSinceAttack --> should probably be a pointer to processor variable
     *      i) calculate & apply blockTargetGain (bTG will be our return value
     * 2) When the attack has been over
     * 3) When release is engaged
     * 4) When release is continuing to engage
     *
     * -- DEFINED VARIABLES --
     *
     *
     */
    
    
/* We should only need the following for this class
 *      currentRMS, currentThreshold, currentGain, NoSamplesApplyGain, attackFlag
 *          The mathematical functions will be calculated separately
 */
    
    // This should return blockTargetGain
    // Add block size to timeSinceAttack After this
    float beginAttack(float currentGain, float numberOfSamplesToApplyGain, float currentRMS,
                      float thresholdRMS, bool attackFlag, float* ratio)
    {
        initCurrentGain(currentGain);
        attackFlag = true;
        timeSinceAttack = 0;
        overshoot = calculateOvershoot(currentRMS, thresholdRMS);
        ratioHandler(ratio);
        desiredGain = calculateDesiredGain(overshoot, thresholdRMS, *ratio);
        gainFactor = calculateGainFactor(desiredGain, currentRMS);
        
        attackRampProgress(timeSinceAttack, numberOfSamplesToApplyGain, gainFactor,
                           blockTargetGain, startingGain);
        return blockTargetGain;
    }
    
    void continueAttack(float timeSinceAttack, float numberOfSamplesToApplyGain)
    {
        
    }
    
    void beginRelease()
    {
        
    }
    
    void continueRelease()
    {
        
    }
    
    void attackRampProgress(float timeSinceAttack, float numberOfSamplesToApplyGain,
                            float gainFactor, float blockTarget, float gainRightNow)
    {
        if (numberOfSamplesToApplyGain == 0)
            blockTarget = gainFactor;
        else
        {
            float currentProgress = timeSinceAttack / numberOfSamplesToApplyGain;
            gainRange = gainRightNow - gainFactor;
            blockTarget = startingGain - (currentProgress * gainRange);
        }
    }
    
    void ratioHandler(float* ratio)
    {
        if (*ratio == 0.0f)
            *ratio = 1.0f;
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
    float gainFactor;
    float desiredGain;
    float overshoot;
    float startingGain;
    float gainRange;
    float blockTargetGain;
    
    bool attackFlag;
};

