/*
  ==============================================================================

    CompressorProcessor.cpp
    Created: 4 Aug 2018 8:17:19pm
    Author:  Erik Whipp

  ==============================================================================
*/

#include "CompressorProcessor.h"

CompressorProcessor::CompressorProcessor (float parentSampleRate, int userDeterminedBlockSize)
    : m_blockSize (userDeterminedBlockSize), m_sampleRate (parentSampleRate) { }

CompressorProcessor::~CompressorProcessor() { }

float CompressorProcessor::beginAttack (float startingGainFactor, float ratioSlider, float attackSlider,
                                 float currentOvershoot, float thresholdRMS, float currentRMS)
{
    setStartingGainFactor (startingGainFactor);
    setTimeSinceAttack (0);

    m_numberOfSamplesToApplyGain = calculateNumSamples (attackSlider, m_sampleRate, m_blockSize);
    m_desiredGainFactor          = calculateDesiredGain (currentOvershoot, thresholdRMS, ratioSlider);
    m_gainFactor                 = calculateGainFactor (m_desiredGainFactor, currentRMS);
    
    m_timeSinceAttack += m_blockSize;
    
    if (m_numberOfSamplesToApplyGain == 0)
        return m_gainFactor;
    else
        return startingGainFactor - ((m_timeSinceAttack / m_numberOfSamplesToApplyGain) * (startingGainFactor - m_gainFactor));
}

float CompressorProcessor::continueAttack()
{
    m_timeSinceAttack += m_blockSize;
    
    if ((m_timeSinceAttack / m_numberOfSamplesToApplyGain) >= 1)
        return m_gainFactor;
    else
        return m_startingGainFactor - ((m_timeSinceAttack / m_numberOfSamplesToApplyGain) * m_gainFactor);
}

float CompressorProcessor::beginRelease(float startingGainFactor, float releaseSlider)
{
    setStartingGainFactor (startingGainFactor);
    setTimeSinceRelease (0);
    
    m_gainFactor = 1.0f;
    m_numberOfSamplesToApplyGain = calculateNumSamples (releaseSlider, m_sampleRate, m_blockSize);
    m_timeSinceRelease += m_blockSize;
    
    if (m_numberOfSamplesToApplyGain == 0)
        return m_gainFactor;
    else
        return startingGainFactor + ((m_timeSinceRelease / m_numberOfSamplesToApplyGain) * (m_gainFactor - startingGainFactor));
}

float CompressorProcessor::continueRelease()
{
    m_timeSinceRelease += m_blockSize;
    
    if ((m_timeSinceRelease / m_numberOfSamplesToApplyGain) >= 1)
        return m_gainFactor;
    else
        return m_startingGainFactor + ((m_timeSinceRelease / m_numberOfSamplesToApplyGain) * m_gainFactorRange);
}

void CompressorProcessor::setKneeType (bool isActive, float thresholdSlider, float ratioSlider, float incomingSignal, int kneeWidth)
{
    if (m_isKneeActive)
    {
        engageHardKnee (thresholdSlider, ratioSlider, incomingSignal, kneeWidth);
    }
    else { return; }
}
