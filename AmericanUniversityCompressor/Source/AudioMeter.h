/*
  ==============================================================================

    AudioMeter.h
    Created: 24 Jan 2018 8:19:26pm
    Author:  Erik Whipp

   This is a simple helper because the JUCE library does not
   provide a client facing level meter.
 
   Areas of use:
   RMS and dBvalue meters.
  ==============================================================================
*/

#pragma once
#ifndef AudioMeter_h
#define AudioMeter_h
#endif /* AudioMeter_h */

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <iomanip>
#include <string>

class AudioMeter : public Component
{
public:
    AudioMeter() {}
    
    ~AudioMeter() {}
    
    void paint(Graphics &g) override
    {
        g.fillAll(Colours::black);
        
        g.addTransform(AffineTransform::verticalFlip((float)getHeight()));
        g.setColour(Colours::red);
        
        // Showing the actual levels -- Second is for dB as it is inherently less than 0.
        
        if (level >= 0)
            g.fillRect(0.0f, 0.0f, (float) getWidth(), (float) getHeight() * level);
        if (level < 0)
            g.fillRect(0.0f, 0.0f, (float) getWidth(), ((float) getHeight() * level) + 100);
    }
    
    // The primary function, limits the level between 0.0 and 1.0, then repaints the component.
    void setLevelRMS(float newLevel)
    {
        level = newLevel;
        level = jlimit(0.0f, 1.0f, newLevel);
        repaint();
    }
    
    void setLeveldB(float newLevel)
    {
        level = newLevel;
        level = jlimit(0.0f, 100.0f, newLevel);
        repaint();
    }

private:
    float level;
};
