/*
  ==============================================================================

    TextFormattedSlider.h
    Created: 26 Mar 2018 5:03:49pm
    Author:  Erik Whipp

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TextFormatSlider : public Slider
{
public:
    enum
    {
        None = 0,
        Level,
        Gain,
        Milliseconds,
        Ratio,
        Hertz
    };
    
    TextFormatSlider (SliderStyle style, TextEntryBoxPosition pos, int typeOfOutput=0) : Slider (style, pos),
                                                                                         type (typeOfOutput)
    {}
    
    // Method to create some arbitrary meaning out of the value of our slider. (For users)
    String getTextFromValue (double value) override
    {
        switch (type)
        {
            case Level:
                return String (value, 1) + " dB";
            case Gain:
                return String (Decibels::gainToDecibels (value, -80.0), 1) + " dB";
            case Milliseconds:
                if (value >= 1000.0)
                    return String ((value / 1000), 2) + " s";
                else
                    return String (roundToInt (value)) + " ms";
            case Ratio:
                return "1:" + String (value, 0);
            case Hertz:
                if (value >= 1000.0)
                    return String (value * 0.001, 2) + " khZ";
                else
                    return String (value, 0) + " Hz";
            default:
                return Slider::getTextFromValue (value);
        }
    }
    
    // What our slider values will ultimately be.
    double getValueFromText (const String& text) override
    {
        switch (type) {
            case Level:
                return text.trimCharactersAtEnd (" dB").getFloatValue();
            case Gain:
                return Decibels::decibelsToGain (text.trimCharactersAtEnd (" dB").getFloatValue(), -80.0f);
            case Milliseconds:
                if (text.endsWith ("ms"))
                    return text.trimCharactersAtEnd (" ms").getFloatValue() * 0.001f;
                else
                    return text.trimCharactersAtEnd (" s").getFloatValue();
            case Ratio:
                return text.trimCharactersAtStart ("1:").getFloatValue();
            case Hertz:
                if (text.endsWith ("kHz"))
                    return text.trimCharactersAtEnd (" kHz").getFloatValue() * 1000.0;
                else
                    return text.trimCharactersAtEnd (" Hz").getFloatValue();
            default:
                return Slider::getValueFromText (text.trimCharactersAtEnd (" %"));
        }
    }
    
    void setSliderType (const int t) { type = t; }
    
private:
    int type = None;
};
