/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "AudioMeter.h"
#include "TextFormatSlider.h"



//==============================================================================
/**
*/
class AmericanUniversityCompressorAudioProcessorEditor  : public AudioProcessorEditor,
                                                          private Timer
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    enum
    {
        parameterControlHeight = 40,
        paramaterControlLabelWidth = 80,
        parameterControlWidth = 300
    };
    
    AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor&, AudioProcessorValueTreeState&);
    ~AmericanUniversityCompressorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
private:
    // Essentials
    AmericanUniversityCompressorAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    // Sliders/Labels
    Label makeupGainLabel;
    ScopedPointer<TextFormatSlider> makeupGainSlider;
    ScopedPointer<SliderAttachment> makeupAttachment;
    
    Label thresholdLabel;
    ScopedPointer<TextFormatSlider> thresholdSlider;
    ScopedPointer<SliderAttachment> thresholdAttachment;
    
    Label attackLabel;
    ScopedPointer<TextFormatSlider> attackSlider;
    ScopedPointer<SliderAttachment> attackAttachment;
    
    Label releaseLabel;
    ScopedPointer<TextFormatSlider> releaseSlider;
    ScopedPointer<SliderAttachment> releaseAttachment;
    
    Label ratioLabel;
    ScopedPointer<TextFormatSlider> ratioSlider;
    ScopedPointer<SliderAttachment> ratioAttachment;
   
    // Meters
    AudioMeter rmsValue;
    Label rmsValueLabel;
    
    AudioMeter rms2DBValue;
    Label rms2DBValueLabel;
    
    // Debugging
    Label currentGainEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessorEditor)
};
