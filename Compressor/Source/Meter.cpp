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
        case (METER_TYPE::LEVEL):         drawBasicMeter (g); break;
        case (METER_TYPE::VISUAL):     /* drawVisual */ break;
        case (METER_TYPE::ENGAGEMENT):    drawEngagement (g); break;
        case (METER_TYPE::RMS):           drawBasicMeter (g); break;
        case (METER_TYPE::MAX_METER_TYPES): throw MeterInitializationException ("Error creating meter. Please choose the correct meter you desire.");
        default: break;
    }
    
    DBG ("Meter Initialized");
}

void Meter::drawBasicMeter (Graphics& g)
{
    g.setColour (Colours::black);
    auto bounds = getLocalBounds().toFloat();
    g.fillRect (bounds);
    
    g.setColour (Colours::red);
    g.addTransform (AffineTransform::verticalFlip ((float) bounds.getHeight()));
    g.fillRect (bounds.getX(), bounds.getY(), (float) bounds.getWidth(), (float) bounds.getHeight() * normalize (m_incomingSignal));
    DBG ("END DRAW BASIC METER");
}

void Meter::drawVisual (Graphics& g)
{
    
}

void Meter::drawEngagement (Graphics& g)
{
    DBG ("Engagement meter initialized");
    
    g.setColour (Colours::black);
    auto bounds = getLocalBounds().toFloat();
    g.fillRect(bounds);
    
    g.setColour (Colours::red);
    Point <float> initCenterPoint   (bounds.getWidth() / 2, bounds.getHeight());
    Point <float> initEndPoint      (0, bounds.getWidth());
    Line  <float> initArrow         (initCenterPoint, initEndPoint);
    
    g.drawArrow (initArrow, 2.0f, 10.0f, 10.0f);
    
    auto signal = normalize (m_incomingSignal, 0.0f, 1.0f);
    
    if (signal >= 0.0f && 1.0 >= signal)
        initEndPoint.applyTransform (AffineTransform::rotation (normalize (m_incomingSignal, 0.0f, 1.0f)));
}

float Meter::normalize (float incomingSignal)
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

float Meter::normalize (float incomingSignal, float maximumValue, float minimumValue)
{
    return (incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue);
}

void Meter::setIncomingSignal (float value)
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
