/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EchoAudioProcessorEditor::EchoAudioProcessorEditor (EchoAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p),
      processor (p),
      valueTreeState(vts),
      delay_ramp_slider     (Slider::LinearVertical, Slider::TextBoxBelow),
      delay_feedback_slider (Slider::LinearVertical, Slider::TextBoxBelow),
      delay_time_slider     (Slider::LinearVertical, Slider::TextBoxBelow),
      delay_toggle_btn      ("Toggle Echo")
{
    setSize (400, 300);
}

EchoAudioProcessorEditor::~EchoAudioProcessorEditor()
{
}

//==============================================================================
void EchoAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void EchoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
