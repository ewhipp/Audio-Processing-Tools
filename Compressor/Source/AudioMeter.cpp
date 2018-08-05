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

AudioMeter::AudioMeter() { }
AudioMeter::~AudioMeter() { }

void AudioMeter::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    
    g.addTransform(AffineTransform::verticalFlip((float)getHeight()));
    g.setColour(Colours::red);
        
    g.fillRect(0.0f, 0.0, (float) getWidth(), (float) getHeight() * normalize(level));
    
}

float AudioMeter::normalize (float incomingSignal)
{
    switch (type)
    {
        case RMS: return (incomingSignal - ((0.0f + 1.0f) / 2) / ((1.0 - 0.0f) / 2)); break;
        case Level: return (incomingSignal - ((-100.0f + 0.0f) / 2) / ((0.0f - -100.f) / 2)); break;
        default: return 0; break;
    }
}

void AudioMeter::setLevel (float newLevel, float minimumValue, float maximumValue)
{
    level = newLevel;
    level = jlimit (minimumValue, maximumValue, newLevel);
    repaint();
}

void AudioMeter::setType (int n) { type = n; }

