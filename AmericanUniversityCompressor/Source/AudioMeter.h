/*
  ==============================================================================

    AudioMeter.h
    Created: 24 Jan 2018 8:19:26pm
    Author:  Erik Whipp

  ==============================================================================
*/

#pragma once
//
//  AudioMeter.h
//  AmericanUniversityCompressor
//
//  Created by Erik Whipp on 1/24/18.
//  This is a simple helper because the JUCE library does not
//  provide a client facing level meter

#ifndef AudioMeter_h
#define AudioMeter_h


#endif /* AudioMeter_h */
#include "../JuceLibraryCode/JuceHeader.h"

class AudioMeter : public Component
{
public:
    AudioMeter()
    {
        level = 0.0f;
    }
    
    ~AudioMeter() {}
    
    void paint(Graphics &g) override
    {
        g.fillAll(Colours::black);
        
        g.addTransform(AffineTransform::verticalFlip((float)getHeight()));
        g.setColour(Colours::red);
        
        // Showing the actual levels
        g.fillRect(0.0f, 0.0f, (float)getWidth(), (float)getHeight() * level);
    }
    
    // The primary function, limits the level between 0.0 and 1.0, then repaints the
    // component.
    void setLevel(float newLevel)
    {
        level = jlimit(0.0f, 1.0f, newLevel);
        repaint();
    }
private:
    float level;
};
