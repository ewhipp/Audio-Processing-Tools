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
   
    /*
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
    float beginAttack(float, float, float, float, float, float);
    
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
    float beginRelease(float, float);
    
    /*
     * Represents when the signal stream value continues to be below it's natural
     * level. The compressor is still de-compressor to the natural state.
     *
     * @return: The increased signal level that should be reached after one block size.
    */
    float continueRelease();
    
    // Setters
    void setKneeType (bool);
    
private:
    
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
    int calculateNumSamples (float ratioSlider, int parentSampleRate, int blockSize)
    {
        float sliderValue = ratioSlider / 1000;
        float timeToWaste = sliderValue * parentSampleRate;

        return (blockSize > timeToWaste) ? 0.0f : timeToWaste;
    }
    
    /**
     * A knee in compressor terms determines how effectively the compressor
     * works when the threshold is breached. A hard knee will act as though
     * the compressor is working normally. The softer the knee, the less
     * intensive the compressor will react to crossing the threshold.
     *
     * EXAMPLE:
     * signals far below the Threshold are uncompressed (ratio of 1:1) < Knee width
     * signals a bit below the Threshold are a bit compressed (ratio of 2:1) threshold signal - Knee width / 2
     * signals at or right around the Threshold are more compressed (ratio of 3:1) threshold signal + Knee width / 2
     * signals far above the Threshold are fully compressed (ratio of 4:1) > Knee width
     *
     * TWO STATES: Soft vs. Hard
     * Soft: The compressor acts normally as if there is no knee.
     * Hard: @see example above.
     *
     * @param thresholdSlider: The threshold slider's value.
     * @param rationSlider: The ratio slider's value.
     * @param incomingSignal: The current stream of audio.
     * @param kneeWidth: The window of affected audio.
     */
    float engageHardKnee (float thresholdSlider, float ratioSlider, float incomingSignal, int kneeWidth)
    {
        if (applyHardKnee)
        {
            if (incomingSignal > thresholdSlider + kneeWidth)
                return ratioSlider / 1000;
            else if (thresholdSlider + (kneeWidth / 2) >= incomingSignal && incomingSignal > thresholdSlider - (kneeWidth / 2))
                return (ratioSlider - 1) / 1000;
            else if (thresholdSlider - 3 >= incomingSignal && incomingSignal > thresholdSlider - 7)
                return (ratioSlider  - 2) / 1000;
            else
                return 1.0f;
        }
        
        return 1.0f;
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
    
    bool applyHardKnee;
};

