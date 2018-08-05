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
    CompressorProcessor(float, int);
    
    ~CompressorProcessor();
   
    /* TODO: Refactor to smaller functions
     *
     * Represents when our current samples are going over the threshold.
     * This function will automatically engage the calculations when a
     * signal stream value has surpassed the current threshold.
     *
     * @param: The starting gain factor or target gain the compressor currently resides at.
     * @param: The ratio slider's current value.
     * @param: The attack slider's current value.
     * @param: The current amount that the incoming signal exceeds the threshold slider by.
     * @param: The RMS value of the threshold slider.
     * @param: The RMS value of the incoming signal.
     *
     * @return: The decreased signal level that should be reached after one block size.
     */
    float beginAttack(float, float*, float*, float, float, float);
    
    /* 
     * Represents when our overshoot has not been recalculated and we are still
     * attacking with the same parameters set by the beginAttack() function.
     *
     * @return: The decreased signal level that should be reached after one block size.
     */ 
    float continueAttack ();
    
    /*
     * Represents when the signal stream value has reached the level the user
     * has set based on the knob parameters. This signifies that the compressor
     * must begin to de-compress, allowing the signal to go back to it's natural
     * level.
     *
     * @param: The starting gain factor or target gain the compressor currently resides at.
     * @param: The release slider's current value.
     *
     * @return: The increased signal level that should be reached after one block size.
     */ 
    float beginRelease(float, float*);
    
    /*
     * Represents when the signal stream value continues to be below it's natural
     * level. The compressor is still de-compressor to the natural state.
     *
     * @return: The increased signal level that should be reached after one block size.
     */ 
    float continueRelease();
    
private:
    
    /* Residual calculations */
    
    /*
     * Calculate the difference between the current signal stream value and the current
     * value of the threshold meter.
     *
     * @param: The incoming signal value as an RMS value
     * @param: The value of the threshold slider.
     *
     * @return: The current overshoot.
     */ 
    float calculateOvershoot (float currentRMS, float currentThreshold)
    {
        return currentRMS - currentThreshold;
    }
    
    /*
     * Calculate how much the signal stream should be reduced in relation to the
     * current parameters.
     *
     * @param: The current amount that the incoming signal exceeds the threshold slider by.
     * @param: The value of the threshold slider.
     * @param: The value of the ratio slider.
     *
     * @return: The desired gain the signal stream should be.
     */ 
    float calculateDesiredGain (float currentOvershoot, float currentThreshold,
                                float currentRatio)
    {
        return (currentOvershoot / currentRatio) + currentThreshold;
    }
    
    /*
     * Calculate the fraction that should be multipled with the current signal to
     * attenuate the signal.
     *
     * @param: The calculated desired gain. @see calculateDesiredGain
     * @param: The incoming signal value as an RMS value.
     *
     * @return: The fraction at which the compressed signal must be output.
     */ 
    float calculateGainFactor (float desiredGain, float currentRMS)
    {
        return desiredGain / currentRMS;
    }
    
    /* 
     * Calculate the amount of samples it should take to reach the compressed
     * or decompressed signal value dictated by the compressor's parameters.
     *
     * @param: The value of the ratio slider.
     * @param: The parent session's sample rate or as defined by the user.
     * @param: The parent session's block size or as defined by the user.
     *
     * @return: The number of blocks to wait.
     */
    int calculateNumSamples (float* ratioSlider, int parentSampleRate, int blockSize)
    {
        float sliderValue = *ratioSlider / 1000;
        float timeToWaste = sliderValue * parentSampleRate;

        return (blockSize > timeToWaste) ? 0.0f : timeToWaste;
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
    int getBlockSize() const
    {
        return blockSize;
    }
    
    int getParentSampleRate() const
    {
        return sampleRate;
    }
    
    int getTimeSinceRelease() const
    {
        return timeSinceRelease;
    }
    
    int getTimeSinceAttack() const
    {
        return timeSinceAttack;
    }
    
    float getStartingGainFactor() const
    {
        return startingGainFactor;
    }
    
    // VAR
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

