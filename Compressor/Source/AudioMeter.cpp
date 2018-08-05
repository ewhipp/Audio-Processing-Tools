/*
  ==============================================================================

    AudioMeter.cpp
    Created: 4 Aug 2018 8:17:24pm
    Author:  Erik Whipp

    This is a simple helper because the JUCE library does not
    provide a client facing level meter.
 
    The audio meter is a simple vertical rectangle that fills
    with red color based on the value given to it.
  ==============================================================================
*/

#include "AudioMeter.h"

AudioMeter::AudioMeter (int typeOfMeter) { type = typeOfMeter; }
AudioMeter::~AudioMeter() { }

void AudioMeter::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    g.setColour(Colours::red);
    g.addTransform(AffineTransform::verticalFlip((float)getHeight()));
    
    g.fillRect(0.0f, 0.0, (float) getWidth(), (float) getHeight() * normalize (level));
    std::cout << level << "\n";
}

float AudioMeter::normalize (float incomingSignal)
{
    switch (type)
    {
        case RMS: return (incomingSignal - minimumValue) / (maximumValue - minimumValue); break;
        case Level: return (abs (incomingSignal) - minimumValue) / (minimumValue - maximumValue); break;
        default: return 0; break;
    }
}

void AudioMeter::setLevel (float newLevel, float minimumValue, float maximumValue)
{
    switch (type)
    {
        case RMS: minimumValue = 0.0f; maximumValue = 1.0f; break;
        case Level: minimumValue = 0.0f; maximumValue = 100.0f; break;
        default: break;
    }
    
    level = abs (jlimit (minimumValue, maximumValue, newLevel));
    repaint();
}

void AudioMeter::setType (int n) { type = n; }

