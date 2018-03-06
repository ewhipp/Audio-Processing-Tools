/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
AmericanUniversityCompressorAudioProcessorEditor::AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor& parent,
                                                                                                    AudioProcessorValueTreeState &vts)
:   AudioProcessorEditor (&parent), processor (parent), valueTreeState(vts)
{
    // Labels & Slider init
    // rotary sliders
    attackLabel.setText("Attack", dontSendNotification);
    addAndMakeVisible(attackLabel);
    attackSlider = new Slider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
    addAndMakeVisible (attackSlider);
    attackAttachment = new SliderAttachment (valueTreeState, "attack", *attackSlider);
    
    releaseLabel.setText ("Release", dontSendNotification);
    addAndMakeVisible (releaseLabel);
    releaseSlider = new Slider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
    addAndMakeVisible(releaseSlider);
    releaseAttachment = new SliderAttachment (valueTreeState, "release", *releaseSlider);
    
    ratioLabel.setText ("Ratio", dontSendNotification);
    addAndMakeVisible (ratioLabel);
    ratioSlider = new Slider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
    addAndMakeVisible (ratioSlider);
    ratioAttachment = new SliderAttachment (valueTreeState, "ratio", *ratioSlider);
    
    // horizontal Sliders
    makeupGainLabel.setText("Make-up", dontSendNotification);
    addAndMakeVisible(makeupGainLabel);
    makeupGainSlider = new Slider (Slider::LinearVertical, Slider::TextBoxBelow);
    addAndMakeVisible(makeupGainSlider);
    makeupAttachment = new SliderAttachment (valueTreeState, "makeUpGain", *makeupGainSlider);
    
    thresholdLabel.setText("Threshold", dontSendNotification);
    addAndMakeVisible(thresholdLabel);
    thresholdSlider = new Slider (Slider::LinearVertical, Slider::TextBoxBelow);
    addAndMakeVisible(thresholdSlider);
    thresholdAttachment = new SliderAttachment (valueTreeState, "threshold", *thresholdSlider);
    
    addAndMakeVisible(rmsValue);
    addAndMakeVisible(rmsValueLabel);
    addAndMakeVisible(rms2DBValue);
    addAndMakeVisible(rms2DBValueLabel);
    addAndMakeVisible(currentGainEditor);
    
    startTimerHz(30);
    setSize(580, 350);
}

// Destructor
AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
    thresholdAttachment = nullptr;
    makeupAttachment = nullptr;
    ratioAttachment = nullptr;
    attackAttachment = nullptr;
    releaseAttachment = nullptr;
    ratioSlider = nullptr;
    attackSlider = nullptr;
    releaseSlider = nullptr;
}


 // This function handles the acquisition of the slider values/ the changes.

// Repaint the meters. For more info on meter creation see AudioMeter.h
void AmericanUniversityCompressorAudioProcessorEditor::timerCallback()
{
    // Update rms meter
    rmsValue.setLevelRMS(processor.currentRMS);
    rmsValueLabel.setText(juce::String(processor.currentRMS, 3),
                          dontSendNotification);

    // Update the dB meter
    rms2DBValue.setLeveldB(abs(processor.currentdB));
    rms2DBValueLabel.setText(Decibels::toString(processor.currentdB),
                             dontSendNotification);
    
    currentGainEditor.setText(std::to_string(processor.currentGain) + " current gain", dontSendNotification);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (12.0f);
    g.drawFittedText ("Audio Compressor Example", getLocalBounds(), Justification::centredTop, 1);
}

void AmericanUniversityCompressorAudioProcessorEditor::resized()
{
    Rectangle<int> pluginWindow = getLocalBounds();
    currentGainEditor.setBounds(pluginWindow.removeFromTop(25).removeFromLeft(100));

    auto MeterArea = pluginWindow.removeFromLeft(100);
    auto sliderLabelArea = pluginWindow.removeFromTop(50);
    thresholdLabel.setBounds(sliderLabelArea.removeFromLeft(100));
    thresholdSlider->setBounds(pluginWindow.removeFromLeft(100));
    makeupGainLabel.setBounds(sliderLabelArea.removeFromRight(70));
    currentGainEditor.setBounds(sliderLabelArea.removeFromRight(100));
    makeupGainSlider->setBounds(pluginWindow.removeFromRight(80));
    
    auto LabelArea = MeterArea.removeFromTop(25);
    auto parameterArea = pluginWindow.removeFromBottom(100);
    auto parameterLabelArea = pluginWindow.removeFromBottom(25);
    auto currentGainArea = pluginWindow.removeFromBottom(50);
    
    
    ratioLabel.setBounds(parameterLabelArea.removeFromRight(70));
    ratioSlider->setBounds(parameterArea.removeFromRight(100));
    attackLabel.setBounds(parameterLabelArea.removeFromRight(100));
    attackSlider->setBounds(parameterArea.removeFromRight(100));
    releaseLabel.setBounds(parameterLabelArea.removeFromRight(110));
    releaseSlider->setBounds(parameterArea.removeFromRight(100));
    
    rmsValue.setBounds(MeterArea.removeFromLeft(50));
    rmsValueLabel.setBounds(LabelArea.removeFromLeft(50));
    rms2DBValue.setBounds(MeterArea.removeFromLeft(50));
    rms2DBValueLabel.setBounds(LabelArea.removeFromLeft(50));
}
