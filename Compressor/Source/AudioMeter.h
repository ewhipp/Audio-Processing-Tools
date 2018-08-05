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
    AudioMeter();
    ~AudioMeter();
    
    void paint(Graphics &g) override;
    void setLevel(float, float, float);

private:
    
    float level;
    
    /**
     * Fills the meter based on the signal coming into the meter.
     *
     * @param signalLevel : Current incoming signal.
     */
    void fillMeter (float signalLevel)
    {
        
    }
};
