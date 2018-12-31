/*
  ==============================================================================

    Meter.cpp
    Created: 30 Dec 2018 12:57:35am
    Author:  Erik Whipp

  ==============================================================================
*/

#include "Meter.h"

void Meter::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    g.setColour (Colours::red);
    
    g.addTransform (AffineTransform::verticalFlip ((float) getHeight()));
    
    g.fillRect (0.0f, 0.0f, (float) getWidth(), (float) getHeight() * normalize (m_incomingSignal));
}

float Meter::normalize(float incomingSignal)
{
    return abs ((incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue));
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

const void Meter::setType (int type = METER_TYPE::AUDIO) noexcept
{
    if (static_cast<int>(type) > static_cast<int>(METER_TYPE::MAX_METER_TYPES))
        throw MeterInitializationException ("Meter type must be accepted value");
    
    m_type = static_cast<METER_TYPE>(type);
}

const Meter Meter::reconstructMeter (METER_TYPE& type)
{
    DBG (type);
    
    switch (type)
    {/*
        case (METER_TYPE::AUDIO):      return Meter (METER_TYPE::AUDIO); break;
        case (METER_TYPE::RMS):        return RMSMeter (METER_TYPE::RMS); break;
        case (METER_TYPE::VISUAL):     return Meter (METER_TYPE::VISUAL); break;
        case (METER_TYPE::ENGAGEMENT): return Meter (METER_TYPE::ENGAGEMENT); break;
        case (METER_TYPE::MAX_METER_TYPES) : break;
        default: break;
      */
    }
}
