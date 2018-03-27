/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
ConvolutionReverbAudioProcessorEditor::ConvolutionReverbAudioProcessorEditor (ConvolutionReverbAudioProcessor& parent, AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor (&parent), processor (parent), valueTreeState(vts)
{
    pre_delay.setText("Pre-Delay", dontSendNotification);
    addAndMakeVisible(pre_delay);
    preDelaySlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 3);
    addAndMakeVisible (preDelaySlider);
    preDelayAttachment = new SliderAttachment (valueTreeState, "pre_delay", *preDelaySlider);
    
    size.setText ("Size", dontSendNotification);
    addAndMakeVisible (size);
    sizeSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible(sizeSlider);
    sizeAttachment = new SliderAttachment (valueTreeState, "size", *sizeSlider);
    
    dry.setText ("Dry", dontSendNotification);
    addAndMakeVisible (dry);
    drySlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible (drySlider);
    dryAttachment = new SliderAttachment (valueTreeState, "dry", *drySlider);
    
    gain.setText("Gain", dontSendNotification);
    addAndMakeVisible(gain);
    gainSlider = new TextFormatSlider (Slider::LinearVertical, Slider::TextBoxBelow, 1);
    addAndMakeVisible(gainSlider);
    gainAttachment = new SliderAttachment (valueTreeState, "gain", *gainSlider);
    
    wet.setText("Wet", dontSendNotification);
    addAndMakeVisible(wet);
    wetSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible(wetSlider);
    wetAttachment = new SliderAttachment (valueTreeState, "wet", *wetSlider);
    
    width.setText("Width", dontSendNotification);
    addAndMakeVisible(width);
    widthSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible(widthSlider);
    widthAttachment = new SliderAttachment (valueTreeState, "width", *widthSlider);
    
    freeze.setText("Freeze", dontSendNotification);
    addAndMakeVisible(freeze);
    freezeSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible(freezeSlider);
    freezeAttachment = new SliderAttachment (valueTreeState, "freeze", *freezeSlider);
    
    damping.setText("Damping", dontSendNotification);
    addAndMakeVisible(damping);
    dampingSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible(dampingSlider);
    dampingAttachment = new SliderAttachment (valueTreeState, "damping", *dampingSlider);
    
    processor.addChangeListener (this);
    setSize (780, 500);
}

ConvolutionReverbAudioProcessorEditor::~ConvolutionReverbAudioProcessorEditor()
{
    dryAttachment = nullptr;
    wetAttachment = nullptr;
    gainAttachment = nullptr;
    sizeAttachment = nullptr;
    preDelayAttachment = nullptr;
    widthAttachment = nullptr;
    dampingAttachment = nullptr;
    freezeAttachment = nullptr;
    
    preDelaySlider = nullptr;
    sizeSlider = nullptr;
    drySlider = nullptr;
    gainSlider = nullptr;
    wetSlider = nullptr;
    widthSlider = nullptr;
    dampingSlider = nullptr;
    freezeSlider = nullptr;
}

//==============================================================================
void ConvolutionReverbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Convolution Reverb", getLocalBounds(), Justification::centredTop, 1);
}

void ConvolutionReverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    Rectangle<int> pluginWindow = getLocalBounds();
    auto parameterArea = pluginWindow.removeFromBottom(100);
    auto parameterLabelArea = pluginWindow.removeFromBottom(25);

    pre_delay.setBounds(parameterLabelArea.removeFromLeft(100));
    preDelaySlider->setBounds(parameterArea.removeFromLeft(100));
    
    dry.setBounds(parameterLabelArea.removeFromLeft(100));
    drySlider->setBounds(parameterArea.removeFromLeft(100));
    
    wet.setBounds(parameterLabelArea.removeFromLeft(100));
    wetSlider->setBounds(parameterArea.removeFromLeft(100));
    
    size.setBounds(parameterLabelArea.removeFromLeft(100));
    sizeSlider->setBounds(parameterArea.removeFromLeft(100));
    
    width.setBounds(parameterLabelArea.removeFromLeft(100));
    widthSlider->setBounds(parameterArea.removeFromLeft(100));
    
    damping.setBounds(parameterLabelArea.removeFromLeft(100));
    dampingSlider->setBounds(parameterArea.removeFromLeft(100));
    
    freeze.setBounds(parameterLabelArea.removeFromLeft(100));
    freezeSlider->setBounds(parameterArea.removeFromLeft(100));
    
    gain.setBounds(parameterLabelArea.removeFromLeft(100));
    gainSlider->setBounds(parameterArea.removeFromLeft(100));
    
}


void ConvolutionReverbAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    repaint();
}
