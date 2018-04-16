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
#include "FileBuffer.h"

//==============================================================================
/**
*/
class ConvolutionReverbAudioProcessorEditor  : public AudioProcessorEditor,
                                               public ChangeListener
{
public:
    
    ConvolutionReverbAudioProcessorEditor (ConvolutionReverbAudioProcessor&, AudioProcessorValueTreeState&);
    ~ConvolutionReverbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized () override;
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    
    //========================= User created =======================================
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    void loadButtonClicked ();
    void sampleButtonClicked ();


private:
    // Processor variables
    ConvolutionReverbAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    // Load an audio file
    TextButton loadFileButton;
    
    // View the samples
    TextButton viewSamplesButton;
    
    // File format manager
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    
    // Labels
    Label pre_delay;
    Label dry;
    Label wet;

    // Sliders for the value tree state
    ScopedPointer<TextFormatSlider> preDelaySlider;
    ScopedPointer<SliderAttachment> preDelayAttachment;

    ScopedPointer<TextFormatSlider> drySlider;
    ScopedPointer<SliderAttachment> dryAttachment;
    
    ScopedPointer<TextFormatSlider> wetSlider;
    ScopedPointer<SliderAttachment> wetAttachment;
    
    ReverbProcessor* reverb;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbAudioProcessorEditor)
};
