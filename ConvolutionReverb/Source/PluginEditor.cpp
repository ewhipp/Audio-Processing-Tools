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
    
    dry.setText ("Dry", dontSendNotification);
    addAndMakeVisible (dry);
    drySlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible (drySlider);
    dryAttachment = new SliderAttachment (valueTreeState, "dry", *drySlider);

    wet.setText("Wet", dontSendNotification);
    addAndMakeVisible(wet);
    wetSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 6);
    addAndMakeVisible(wetSlider);
    wetAttachment = new SliderAttachment (valueTreeState, "wet", *wetSlider);
    
    addAndMakeVisible(loadFileButton);
    loadFileButton.setButtonText("Load a convolution file..");
    loadFileButton.onClick = [this] { loadButtonClicked(); };

    processor.addChangeListener (this);
    setSize (300, 200);
}

ConvolutionReverbAudioProcessorEditor::~ConvolutionReverbAudioProcessorEditor()
{
    dryAttachment = nullptr;
    wetAttachment = nullptr;
    preDelayAttachment = nullptr;
    
    preDelaySlider = nullptr;
    drySlider = nullptr;
    wetSlider = nullptr;
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
    Rectangle<int> pluginWindow = getLocalBounds();
    auto parameterArea = pluginWindow.removeFromBottom(100);
    auto parameterLabelArea = pluginWindow.removeFromBottom(25);

    pre_delay.setBounds(parameterLabelArea.removeFromLeft(130));
    preDelaySlider->setBounds(parameterArea.removeFromLeft(100));
    
    dry.setBounds(parameterLabelArea.removeFromLeft(100));
    drySlider->setBounds(parameterArea.removeFromLeft(100));
    
    wet.setBounds(parameterLabelArea.removeFromLeft(35));
    wetSlider->setBounds(parameterArea.removeFromLeft(100));
    
    loadFileButton.setBounds (50, 30, getWidth() - 90, 20);
}


void ConvolutionReverbAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    repaint();
}

// Load a file in for convolution
void ConvolutionReverbAudioProcessorEditor::loadButtonClicked()
{
    // System dialog manager
    FileChooser chosenFile ("Select a .wav file as your impulse response for convolution.",
                            File::nonexistent,
                            "*.wav");
    
    // Notify the thread of a new file to read
   if (chosenFile.browseForFileToOpen())
   {
       auto file = chosenFile.getResult();
       auto pathToFile = file.getFullPathName();
       processor.chosenPath.swapWith (pathToFile);
       
       // Probably should make the threads private
       processor.notify();
       
       FileBuffer::Ptr processAudioBuffer (processor.currentBuffer);
       
       while (true)
           if (processor.currentBuffer != nullptr)
               if (processor.currentBuffer->getAudioSampleBuffer() != nullptr)
               {
                   processor.computeIRFFT();
                   break;
               }
   }
}
