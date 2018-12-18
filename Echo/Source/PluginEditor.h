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
/**
*/
class EchoAudioProcessorEditor  : public AudioProcessorEditor,
                                  public ChangeListener,
                                  private Timer
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    EchoAudioProcessorEditor (EchoAudioProcessor&, AudioProcessorValueTreeState&);
    ~EchoAudioProcessorEditor();

    //==============================================================================
    void timerCallback() override;
    
    //==============================================================================
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    EchoAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    OwnedArray<SliderAttachment> sliderAttachments;
    OwnedArray<ButtonAttachment> buttonAttachments;
    
    Label  delay_ramp_lbl;
    Slider delay_ramp_slider;
    Label  delay_feedback_lbl;
    Slider delay_feedback_slider;
    Label  delay_time_lbl;
    Slider delay_time_slider;
    
    Label  delay_toggle_lbl;
    ToggleButton delay_toggle_btn;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EchoAudioProcessorEditor)
};
