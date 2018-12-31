/*
  ==============================================================================

    EngagementMeter.cpp
    Created: 31 Dec 2018 1:41:36am
    Author:  Erik Whipp

  ==============================================================================
*/

#include "EngagementMeter.h"

void EngagementMeter::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    // Line that follows a half radius
    // The higher the incoming signal, the more the line moves to the left
}

float EngagementMeter::normalize(float incomingSignal)
{
    return (incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue);
}

void EngagementMeter::setValue (float value)
{
    m_incomingSignal = value;
}

const void EngagementMeter::setMinimumValue (float minimumValue)
{
    if (m_maximumValue)
        jassert (m_maximumValue > minimumValue);
    
    m_minimumValue = minimumValue;
}

const void EngagementMeter::setMaximumValue (float maximumValue)
{
    if (m_minimumValue)
        jassert (maximumValue > m_minimumValue);
    
    m_maximumValue = maximumValue;
}


const void EngagementMeter::setType (int type = METER_TYPE::ENGAGEMENT) noexcept
{
    jassert (type == METER_TYPE::ENGAGEMENT);
    
    if (static_cast<int>(type) > static_cast<int>(METER_TYPE::MAX_METER_TYPES))
        throw MeterInitializationException ("Meter type must be accepted value");
        
        m_type = static_cast<METER_TYPE>(type);
}
