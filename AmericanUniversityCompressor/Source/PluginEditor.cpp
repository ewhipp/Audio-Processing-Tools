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
    startTimer(30);
    setSize(580, 350);
}


// Destructor
AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
}


 // This function handles the acquisition of the slider values/ the changes.

// Repaint the meters. For more info on meter creation see AudioMeter.h
void AmericanUniversityCompressorAudioProcessorEditor::timerCallback()
{
    // Update rms meter
    rmsValue.setLevelRMS(processor.currentRMS);
    rmsValueLabel.setText(std::to_string(processor.currentRMS),
                          dontSendNotification);

    // Update the dB meter
    rms2DBValue.setLeveldB(processor.currentdB);
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
    g.drawFittedText ("American University Audio Compressor Example", getLocalBounds(), Justification::centredTop, 1);
}

void AmericanUniversityCompressorAudioProcessorEditor::resized()
{
    Rectangle<int> pluginWindow = getLocalBounds();
    auto someSpace = pluginWindow.removeFromTop(145);
    auto MeterArea = pluginWindow.removeFromLeft(200);
    auto LabelArea = MeterArea.removeFromTop(25);
    
    // Meters
    rmsValue.setBounds(MeterArea.removeFromLeft(100));
    rmsValueLabel.setBounds(LabelArea.removeFromLeft(100));
    rms2DBValue.setBounds(MeterArea.removeFromLeft(100));
    rms2DBValueLabel.setBounds(LabelArea.removeFromLeft(100));
    currentGainEditor.setBounds(0.0, 0.0, 100.0, 100.0);
    ratioSlider.setBounds(50.0, 0.0, 100.0, 100.0);
    ratioLabel.setBounds(150.0, 0.0, 100.0, 100.0);
    
}

