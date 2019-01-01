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
        case (METER_TYPE::ENGAGEMENT):    drawEngagement (g); break;
        case (METER_TYPE::RMS):           drawBasicMeter (g); break;
        case (METER_TYPE::MAX_METER_TYPES): throw MeterInitializationException ("Error creating meter. Please choose the correct meter you desire.");
        default: break;
    }
}

void Meter::drawBasicMeter (Graphics& g)
{
    g.setColour (Colours::black);
    Rectangle <float> bounds = getLocalBounds().toFloat();
    g.fillRect (bounds);
    
    g.addTransform (AffineTransform::verticalFlip ((float) bounds.getHeight()));
    g.setColour (Colours::red);
    
    g.fillRect (bounds.getX(), bounds.getY(), (float) bounds.getWidth(), ((float) bounds.getHeight() * normalize (m_incomingSignal)));
}

void Meter::drawEngagement (Graphics& g)
{
    g.setColour (Colours::black);
    Rectangle <float> bounds = getLocalBounds().toFloat();
    g.fillRect(bounds);
    
    g.setColour (Colours::red);
    Point <float> initCenterPoint   (bounds.getWidth() / 2, bounds.getHeight());
    Point <float> initEndPoint      (0, bounds.getWidth());
    Line  <float> initArrow         (initCenterPoint, initEndPoint);
    g.drawArrow (initArrow, 2.0f, 10.0f, 10.0f);
    
    float signal = normalize (m_incomingSignal, 0.0f, 1.0f);
    
    if (signal >= 0.0f && 1.0f >= signal)
    {
     //   initEndPoint.setX (initEndPoint.getX() + 50);
    }
}

float Meter::normalize (float incomingSignal)
{
    if (m_type == METER_TYPE::LEVEL)
        return abs ((incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue));
    else
        return (incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue);
}

float Meter::normalize (float incomingSignal, float maximumValue, float minimumValue)
{
    return (incomingSignal - m_minimumValue) / (m_maximumValue - m_minimumValue);
}

void Meter::setIncomingSignal (float value)
{
    if (value == m_incomingSignal) { return; } // Don't repaint unless the value has changed.
    
    (value >= m_minimumValue && m_maximumValue > value) ? m_incomingSignal = value : m_incomingSignal = 0.0f;
    repaint();
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
        throw MeterInitializationException ("Meter type must be of a specified type.");
    
    m_type = static_cast<METER_TYPE>(type);
}
