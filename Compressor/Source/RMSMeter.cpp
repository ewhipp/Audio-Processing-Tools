/*
  ==============================================================================

    RMSMeter.cpp
    Created: 30 Dec 2018 1:48:24am
    Author:  Erik Whipp

  ==============================================================================
*/

#include "RMSMeter.h"

void RMSMeter::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    g.setColour (Colours::red);
    
    g.addTransform (AffineTransform::verticalFlip ((float) getHeight()));
    
    g.fillRect (0.0f, 0.0f, (float) getWidth(), (float) getHeight() * normalize (m_incomingSignal));
}

float RMSMeter::normalize(float incomingSignal)
{
    return (incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue);
}

void RMSMeter::setValue (float value)
{
    m_incomingSignal = value;
}

const void RMSMeter::setMinimumValue (float minimumValue)
{
    if (m_maximumValue)
        jassert (m_maximumValue > minimumValue);
    
    m_minimumValue = minimumValue;
}

const void RMSMeter::setMaximumValue (float maximumValue)
{
    if (m_minimumValue)
        jassert (maximumValue > m_minimumValue);
    
    m_maximumValue = maximumValue;
}
