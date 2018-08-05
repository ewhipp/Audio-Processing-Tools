/*
  ==============================================================================

    AudioMeter.h
    Created: 24 Jan 2018 8:19:26pm
    Author:  Erik Whipp

   This is a simple helper because the JUCE library does not
   provide a client facing level meter.
 
   The audio meter is a simple vertical rectangle that fills
   with red color based on the value given to it.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioMeter : public Component
{
public:
    AudioMeter();
    ~AudioMeter();
    
    /**
     * Fill the meter based on the signal level given to the meter.
     */
    void paint(Graphics &g) override;
    
    /**
     * Normalize all incoming signals based on the type of slider.
     *
     * See enum in private: field for more information.
     */
    float normalize (float);
    
    // SETTERS
    void setLevel(float, float, float);
    
    void setType(int);

private:
    
    int type = None;

    float level;
    
    /**
     * Audio meter types that are currently supported.
     * 
     * Audio meters default to no behavior.
     *
     * RMS meter signal values range between 0.0f and 1.0f.
     * Level meter signal values range between -100.0f and 0.0f.
     */
    enum
    {
        None = 0,
        RMS,
        Level
    };
};
