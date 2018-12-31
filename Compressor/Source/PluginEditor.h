/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
class AmericanUniversityCompressorAudioProcessorEditor  : public AudioProcessorEditor,
                                                          public ChangeListener,
                                                          private Timer
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor&);
    ~AmericanUniversityCompressorAudioProcessorEditor();

    //==============================================================================
    
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    
private:
    // Essentials
    AmericanUniversityCompressorAudioProcessor& processor;
    
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
    
    OwnedArray <SliderAttachment> attachments;
    SharedResourcePointer <TooltipWindow> tooltipWindow;
        
    AudioVisualiserComponent signalStreamViewer;
    
   /* std::unique_ptr<AudioMeter> rmsValue;
    Label rmsValueLabel;
    
    std::unique_ptr<AudioMeter> dBMeter;
    Label dBMeterLabel; */
    
    Label engagementLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessorEditor)
};
