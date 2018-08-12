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
    AudioMeter(int);
    ~AudioMeter();
    void paint(Graphics &g) override;
    
    /**
     * Normalize numbers as seen by the following formula:
     *
     *                          y - minimumPossibleValue
     *           x = ------------------------------------------------
     *                  maximumPossibleValue - minimumPossibleValue
     *
     * @return: normalized data basedon the given parameters.
     */
    float normalize(float /* incomingSignal */);
    
    void setVisualMeterLevel (float);
    void setType (int);
    void setMinimumValue (int);
    void setMaximumValue (int);
    void setCurrentLevel (float);
    void setMeterLimits();

private:
    
    /**
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
    
    int type = None;
    float level, minimumValue, maximumValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioMeter)

};
