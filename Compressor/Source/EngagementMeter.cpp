/*
  ==============================================================================

    EngagementMeter.cpp
    Created: 5 Aug 2018 2:18:19pm
    Author:  Erik Whipp

  ==============================================================================
*/

#include "EngagementMeter.h"

EngagementMeter::EngagementMeter (int typeOfMeter)
{
    
}

EngagementMeter::~EngagementMeter()
{
}

void EngagementMeter::paint(Graphics& g)
{
    setVisualMeterLevel (0.7);
}

void EngagementMeter::setVisualMeterLevel (float incomingSignal)
{
    setCurrentLevel (incomingSignal);
    v_Meter.setTransform (AffineTransform::rotation(level));
}

float EngagementMeter::normalize(float incomingSignal)
{
    return (incomingSignal - minimumValue) / (maximumValue - minimumValue);
}

void EngagementMeter::setMaximumValue(float n) { maximumValue = n; }
void EngagementMeter::setMinimumValue(float n) { minimumValue = n; }
void EngagementMeter::setCurrentLevel(float n) { level = n; }
