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
    
    /* 
     * Method to create some arbitrary unit out of the value of our slider.
     * 
     * Examples include: * vs. milli-* vs. micro-*.
     * 
     * @return: The correct unit to append to a label.
     */
    String getTextFromValue (double) override;
    
    /*
     * Method to retrieve the value of a slider without the arbitrary parts.
     *
     * @return: The `important` value to the context of the program.
     */
    double getValueFromText (const String&) override;
    
    /**
     * Designate the type of slider that will be displayed giving context
     * to the type of information that must be displayed under the slider.
     */
    void setSliderType (const int);
    
private:
    
    // DEFAULT TYPE OF SLIDER DOES NOT APPEND ANY INFORMATION.
    int type = None;
    
    /**
     * Sliders can be monitoring level, gain, time, ratios.
     */
    enum
    {
        None = 0,
        Level,
        Gain,
        Milliseconds,
        Ratio
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextFormatSlider)

};
