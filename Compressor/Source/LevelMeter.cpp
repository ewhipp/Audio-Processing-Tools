/*
  ==============================================================================

    LevelMeter.cpp
    Created: 30 Dec 2018 12:57:35am
    Author:  Erik Whipp

    LevelMeter is designed to take an incoming signal and normalize it between
    the range of -100.0
  ==============================================================================
*/

#include "LevelMeter.h"

void LevelMeter::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    g.setColour (Colours::red);
    
    g.addTransform (AffineTransform::verticalFlip ((float) getHeight()));
    
    g.fillRect (0.0f, 0.0f, (float) getWidth(), (float) getHeight() * normalize (m_incomingSignal));
}

float LevelMeter::normalize(float incomingSignal)
{
    return abs ((incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue));
}

void LevelMeter::setValue (float value)
{
    m_incomingSignal = value;
}

const void LevelMeter::setMinimumValue (float minimumValue)
{
    if (m_maximumValue)
        jassert (m_maximumValue > minimumValue);
    
    m_minimumValue = minimumValue;
}

const void LevelMeter::setMaximumValue (float maximumValue)
{
    if (m_minimumValue)
        jassert (maximumValue > m_minimumValue);
    
    m_maximumValue = maximumValue;
}

const void LevelMeter::setType (int type = METER_TYPE::LEVEL) noexcept
{
    jassert (type == METER_TYPE::LEVEL);
    
    if (static_cast<int>(type) > static_cast<int>(METER_TYPE::MAX_METER_TYPES))
        throw MeterInitializationException ("Meter type must be accepted value");
    
    m_type = static_cast<METER_TYPE>(type);
}
