/*
  ==============================================================================

    AudioMeter.cpp
    Created: 4 Aug 2018 8:17:24pm
    Author:  Erik Whipp

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
        
    if (level >= 0)
        g.fillRect(0.0f, 0.0f, (float) getWidth(), (float) getHeight() * level);
    if (level < 0)
        g.fillRect(0.0f, 0.0f, (float) getWidth(), ((float) getHeight() * abs(level) / 100));
}

void AudioMeter::setLevel(float newLevel, float minimumValue, float maximumValue)
{
    level = newLevel;
    level = jlimit (minimumValue, maximumValue, newLevel);
    repaint();
}
