/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "TextFormattedSlider.h"

//==============================================================================
/**
*/
class ConvolutionReverbAudioProcessorEditor  : public AudioProcessorEditor,
                                               public ChangeListener
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    ConvolutionReverbAudioProcessorEditor (ConvolutionReverbAudioProcessor&, AudioProcessorValueTreeState&);
    ~ConvolutionReverbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void changeListenerCallback(ChangeBroadcaster* sender) override;


private:
    // Processor variables
    ConvolutionReverbAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    // Labels
    Label size;
    Label pre_delay;
    Label width;
    Label dry;
    Label wet;
    Label damping;
    Label freeze;
    Label gain;
    
    // Parameters
    ScopedPointer<TextFormatSlider> sizeSlider;
    ScopedPointer<SliderAttachment> sizeAttachment;
    
    ScopedPointer<TextFormatSlider> preDelaySlider;
    ScopedPointer<SliderAttachment> preDelayAttachment;
    
    ScopedPointer<TextFormatSlider> widthSlider;
    ScopedPointer<SliderAttachment> widthAttachment;
    
    ScopedPointer<TextFormatSlider> drySlider;
    ScopedPointer<SliderAttachment> dryAttachment;
    
    ScopedPointer<TextFormatSlider> wetSlider;
    ScopedPointer<SliderAttachment> wetAttachment;
    
    ScopedPointer<TextFormatSlider> gainSlider;
    ScopedPointer<SliderAttachment> gainAttachment;
    
    ScopedPointer<TextFormatSlider> dampingSlider;
    ScopedPointer<SliderAttachment> dampingAttachment;
    
    ScopedPointer<TextFormatSlider> freezeSlider;
    ScopedPointer<SliderAttachment> freezeAttachment;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbAudioProcessorEditor)
};
