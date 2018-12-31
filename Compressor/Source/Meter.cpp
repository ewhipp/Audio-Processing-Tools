/*
  ==============================================================================

    Meter.cpp
    Created: 31 Dec 2018 10:43:10am
    Author:  Erik Whipp

  ==============================================================================
*/

#include "Meter.h"

void Meter::paint (Graphics& g)
{
    switch (m_type)
    {
        case (METER_TYPE::LEVEL): /* drawLevel */ break;
        case (METER_TYPE::VISUAL): /* drawVisual */ break;
        case (METER_TYPE::ENGAGEMENT): /* drawEngagement */ break;
        case (METER_TYPE::RMS): /* drawRMS */ break;
        case (METER_TYPE::MAX_METER_TYPES): throw MeterInitializationException ("Error creating meter. Please choose the correct meter you desire.");
        default: break;
    }
}

float Meter::normalize(float incomingSignal)
{
    if (m_type == METER_TYPE::LEVEL)
    {
        return abs ((incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue));
    }
    else
    {
        return (incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue);
    }
}

void Meter::setValue (float value)
{
    m_incomingSignal = value;
}

const void Meter::setMinimumValue (float minimumValue)
{
    if (m_maximumValue)
        jassert (m_maximumValue > minimumValue);
    
    m_minimumValue = minimumValue;
}

const void Meter::setMaximumValue (float maximumValue)
{
    if (m_minimumValue)
        jassert (maximumValue > m_minimumValue);
    
    m_maximumValue = maximumValue;
}

const void Meter::setType (int type) noexcept
{   
    if (static_cast<int>(type) > static_cast<int>(METER_TYPE::MAX_METER_TYPES))
        throw MeterInitializationException ("Meter type must be accepted value");
    
    m_type = static_cast<METER_TYPE>(type);
}
