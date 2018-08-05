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
#include "util.h"

//==============================================================================
class AmericanUniversityCompressorAudioProcessorEditor  : public AudioProcessorEditor,
                                                          public ChangeListener,
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
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    
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
   
    // Visualize the aduio
    AudioVisualiserComponent audioView;
    // Meters
    AudioMeter rmsValue;
    Label rmsValueLabel;
    
    AudioMeter rms2DBValue;
    Label rms2DBValueLabel;
    
    // Debugging
    Label currentGainEditor;
    Label currentThresholdRMS;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessorEditor)
};
