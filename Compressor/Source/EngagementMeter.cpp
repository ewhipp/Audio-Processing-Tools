/*
  ==============================================================================

    EngagementMeter.cpp
    Created: 5 Aug 2018 2:18:19pm
    Author:  Erik Whipp

  ==============================================================================
*/

#include "EngagementMeter.h"

EngagementMeter::EngagementMeter(int height, int width)
{
    outline.setSize(width, height);
    meterLine.setStart (5, 5);
    meterLine.setEnd (10, 10);
    
    setMinimumValue (outline.getBottom());
    setMaximumValue (outline.getWidth());
}

EngagementMeter::~EngagementMeter()
{
}

void EngagementMeter::paint(Graphics& g)
{
    g.fillAll(Colours::indigo);
    g.setColour(Colours::red);
    
    g.drawRect(outline);
    g.drawLine(meterLine);
}

void EngagementMeter::moveMeter (float compressorEngagement)
{
    meterLine.applyTransform (AffineTransform::scale (normalizeSignal (compressorEngagement)));
}

float EngagementMeter::normalizeSignal(float incomingSignal)
{
    return (incomingSignal - minimumValue) / (maximumValue - minimumValue);
}

void EngagementMeter::setMaximumValue(int n) { maximumValue = n; }
void EngagementMeter::setMinimumValue(int n) { minimumValue = n; }
