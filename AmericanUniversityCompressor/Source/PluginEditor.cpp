/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
AmericanUniversityCompressorAudioProcessorEditor::AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    
    addAndMakeVisible(threshold);
    threshold.setSliderStyle(Slider::LinearVertical);
    threshold.setTextBoxStyle(Slider::TextBoxBelow, true, 125, 25);
    threshold.setRange(-96.0, 0.0);
    threshold.setTextValueSuffix("dB");
    threshold.addListener(this);
    
    addAndMakeVisible(thresholdLabel);
    thresholdLabel.setText("Threshold", dontSendNotification);
    //thresholdLabel.attachToComponent(&threshold, true);
    thresholdLabel.setJustificationType(Justification::centredBottom);
    
    addAndMakeVisible(attackTime);
    attackTime.setSliderStyle(Slider::Rotary);
    attackTime.setRange(0.00, 1000.00);
    attackTime.setTextValueSuffix("ms");
    attackTime.addListener(this);
    attackTime.setTextBoxStyle(Slider::TextBoxBelow, true, 125, 25);
    
    addAndMakeVisible(attackTimeLabel);
    attackTimeLabel.setText("Attack", dontSendNotification);
   // attackTimeLabel.attachToComponent(&attackTime, true);
    attackTimeLabel.setJustificationType(Justification::centredBottom);
    
    addAndMakeVisible(releaseTime);
    releaseTime.setSliderStyle(Slider::Rotary);
   
    releaseTime.setRange(0.00, 1000.00);
    releaseTime.setTextValueSuffix("ms");
    releaseTime.addListener(this);
    releaseTime.setTextBoxStyle(Slider::TextBoxBelow, true, 125, 25);
    addAndMakeVisible(releaseTimeLabel);
    releaseTimeLabel.setText("Release", dontSendNotification);
    releaseTimeLabel.setJustificationType(Justification::centredBottom);
    //releaseTimeLabel.attachToComponent(&releaseTime, true);
    
    
    addAndMakeVisible(makeupGain);
    makeupGain.setSliderStyle(Slider::Rotary);
    makeupGain.setTextBoxStyle(Slider::TextBoxBelow, true, 125, 25);
    makeupGain.setRange(0.00, 100.00);
    makeupGain.setTextValueSuffix("dB");
    makeupGain.addListener(this);
    
    addAndMakeVisible(makeupGainLabel);
    makeupGainLabel.setText("Makeup Gain", dontSendNotification);
    makeupGainLabel.setJustificationType(Justification::centredBottom);
   // makeupGainLabel.attachToComponent(&makeupGain, true);
    
    
    addAndMakeVisible(ratio);
    ratio.setSliderStyle(Slider::Rotary);
    ratio.setTextBoxStyle(Slider::TextBoxBelow, true, 125, 25);
    ratio.setRange(0.00, 1.00, 0.125);
    ratio.addListener(this);
    
    addAndMakeVisible(ratioLabel);
    ratioLabel.setText("Ratio", dontSendNotification);
    ratioLabel.setJustificationType(Justification::centredBottom);
    
    addAndMakeVisible(rmsValue);
    rmsValue.setLevel(0.0f);
    
    addAndMakeVisible(rmsValueLabel);
    rmsValueLabel.setText("RMS", dontSendNotification);
    rmsValueLabel.attachToComponent(&rmsValue, false);
    
    
    addAndMakeVisible(rms2DBValue);
    rms2DBValue.setLevel(0.0f);
    
    addAndMakeVisible(rms2DBValueLabel);
    rms2DBValueLabel.setText("dB", dontSendNotification);
    rms2DBValueLabel.attachToComponent(&rms2DBValue, false);

    
    startTimerHz(30);

    setSize (800, 600);
}
AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
    
}


/*
 * This function handles the acquisition of the slider values/ the changes.
 */
void AmericanUniversityCompressorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
}

void AmericanUniversityCompressorAudioProcessorEditor::timerCallback()
{
    
    rmsValue.setLevel(processor.currentRMS);
    rmsValueLabel.setText(std::to_string(processor.currentRMS),
                          dontSendNotification);

    rms2DBValue.setLevel(processor.currentdB);
    rms2DBValueLabel.setText(std::to_string(processor.currentdB),
                             dontSendNotification);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("American University Audio Compressor Example", getLocalBounds(), Justification::centredTop, 1);
}

void AmericanUniversityCompressorAudioProcessorEditor::resized()
{
    auto pluginWindow = getLocalBounds();
    auto SliderArea = pluginWindow.removeFromBottom(150);
    auto labelArea = pluginWindow.removeFromBottom(25);
   
    thresholdLabel.setBounds(labelArea.removeFromLeft(80));
    threshold.setBounds(SliderArea.removeFromLeft(80));
    
    ratioLabel.setBounds(labelArea.removeFromLeft(80));
    ratio.setBounds(SliderArea.removeFromLeft(80));
    
    attackTimeLabel.setBounds(labelArea.removeFromLeft(80));
    attackTime.setBounds(SliderArea.removeFromLeft(80));
    
    releaseTimeLabel.setBounds(labelArea.removeFromLeft(80));
    releaseTime.setBounds(SliderArea.removeFromLeft(80));
    
    makeupGainLabel.setBounds(labelArea.removeFromLeft(80));
    makeupGain.setBounds(SliderArea.removeFromLeft(80));
    
    rmsValue.setBounds(SliderArea.removeFromRight(40));
    rmsValueLabel.setBounds(labelArea.removeFromLeft(40));
    
    rms2DBValue.setBounds(SliderArea.removeFromRight(40));
    rms2DBValueLabel.setBounds(labelArea.removeFromRight(40));

}
