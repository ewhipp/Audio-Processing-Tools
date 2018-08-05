/*
  ==============================================================================

    TextFormatSlider.h
    Created: 6 Mar 2018 7:31:06am
    Author:  Erik Whipp

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TextFormatSlider : public Slider
{
public:
    TextFormatSlider (SliderStyle, TextEntryBoxPosition, int);
    ~TextFormatSlider();
    
    // Method to create some arbitrary meaning out of the value of our slider. (For users)
    String getTextFromValue (double) override;
    
    // What our slider values will ultimately be.
    double getValueFromText (const String&) override;
    void setSliderType (const int);
    
private:
    int type = None;
    enum
    {
        None = 0,
        Level,
        Gain,
        Milliseconds,
        Ratio
    };
};
