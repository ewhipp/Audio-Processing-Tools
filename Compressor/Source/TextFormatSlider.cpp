/*
  ==============================================================================

    TextFormatSlider.cpp
    Created: 4 Aug 2018 8:17:32pm
    Author:  Erik Whipp

  ==============================================================================
*/

#include "TextFormatSlider.h"

TextFormatSlider::TextFormatSlider(SliderStyle style,
                                   TextEntryBoxPosition pos,
                                   int typeOfOutput = 0) : Slider (style, pos),
                                                           type (typeOfOutput) { }
TextFormatSlider::~TextFormatSlider() { }

double TextFormatSlider::getValueFromText(const String& text)
{
    switch(type)
    {
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
        default:
            return Slider::getValueFromText (text.trimCharactersAtEnd (" %"));
    }
}

String TextFormatSlider::getTextFromValue(double value)
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
        default:
            return Slider::getTextFromValue (value);
    }
}

void TextFormatSlider::setSliderType(const int t) { type = t; }

