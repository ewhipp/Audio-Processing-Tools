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
#include "EngagementMeter.h"

//==============================================================================
class AmericanUniversityCompressorAudioProcessorEditor  : public AudioProcessorEditor,
                                                          public ChangeListener,
                                                          private Timer
{
public:
    
    AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor&, AudioProcessorValueTreeState&);
    ~AmericanUniversityCompressorAudioProcessorEditor();

    //==============================================================================
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    
private:
    // Essentials
    AmericanUniversityCompressorAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    // Sliders / Labels
    Label makeupGainLabel;
    Slider makeupGainSlider;
    
    Label thresholdLabel;
    Slider thresholdSlider;
    
    Label attackLabel;
    Slider attackSlider;
    
    Label releaseLabel;
    Slider releaseSlider;
    
    Label ratioLabel;
    Slider ratioSlider;
    
    OwnedArray<SliderAttachment> sliderAttachments;
    
    
    // Visualize the aduio
    AudioVisualiserComponent signalStreamViewer;
    // Meters
    std::unique_ptr<AudioMeter> rmsValue;
    Label rmsValueLabel;
    
    ScopedPointer<AudioMeter> dBMeter;
    Label dBMeterLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessorEditor)
};
