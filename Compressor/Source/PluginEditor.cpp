/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
AmericanUniversityCompressorAudioProcessorEditor::AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor& parent)
:   AudioProcessorEditor (&parent),
    processor        (parent),
    makeupGainSlider (Slider::LinearVertical, Slider::TextBoxBelow),
    thresholdSlider  (Slider::LinearVertical, Slider::TextBoxBelow),
    attackSlider     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    releaseSlider    (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    ratioSlider      (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    signalStreamViewer (processor.getTotalNumInputChannels())
{
    attackLabel.setText(translate ("Attack"), dontSendNotification);
    addAndMakeVisible (attackLabel);
    addAndMakeVisible (attackSlider);
    
    releaseLabel.setText (translate ("Release"), dontSendNotification);
    addAndMakeVisible (releaseLabel);
    addAndMakeVisible (releaseSlider);
    
    ratioLabel.setText (translate ("Ratio"), dontSendNotification);
    addAndMakeVisible (ratioLabel);
    addAndMakeVisible (ratioSlider);
    
    makeupGainLabel.setText(translate ("Make-up"), dontSendNotification);
    addAndMakeVisible (makeupGainLabel);
    addAndMakeVisible (makeupGainSlider);
    
    thresholdLabel.setText(translate ("Threshold"), dontSendNotification);
    addAndMakeVisible(thresholdLabel);
    addAndMakeVisible(thresholdSlider);

    dBMeter.reset (new AudioMeter (2));
    
    signalStreamViewer.setNumChannels (2);
    signalStreamViewer.setColours (Colours::black, Colours::green);
    addAndMakeVisible (signalStreamViewer);
    
    processor.addChangeListener (this);
    startTimerHz(30);
    setSize(580, 350);
}

AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
    dBMeter             = nullptr;
    engagementMeter     = nullptr;
}

/*
 * Updates:
 *  - The signal stream viewer.
 *  - The dB meter.
 *  - The engagement meter.
 *
 * @see: AudioMeter, EngagementMeter, OpenGLAudioVisualiser
 */
void AmericanUniversityCompressorAudioProcessorEditor::timerCallback()
{
    signalStreamViewer.pushBuffer (processor.getVisualBuffer());
    dBMeter->setVisualMeterLevel  (processor.getCurrentdB());
    dBMeterLabel.setText(Decibels::toString (processor.getCurrentdB()), dontSendNotification);
}

//==============================================================================
void AmericanUniversityCompressorAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (12.0f);
}

void AmericanUniversityCompressorAudioProcessorEditor::resized()
{
    
    Rectangle<int> pluginWindow = getLocalBounds();
    Rectangle<int> sliderLabelArea = pluginWindow.removeFromTop(50);
    
    thresholdLabel.setBounds   (sliderLabelArea.removeFromLeft(100));
    thresholdSlider.setBounds  (pluginWindow.removeFromLeft(100));
    makeupGainLabel.setBounds  (sliderLabelArea.removeFromRight(70));
    makeupGainSlider.setBounds (pluginWindow.removeFromRight(80));
    
    Rectangle<int> parameterArea =      pluginWindow.removeFromBottom(100);
    Rectangle<int> parameterLabelArea = pluginWindow.removeFromBottom(25);
    Rectangle<int> currentGainArea =    pluginWindow.removeFromBottom(50);
    
    ratioLabel.setBounds    (parameterLabelArea.removeFromRight(70));
    ratioSlider.setBounds   (parameterArea.removeFromRight(100));
    attackLabel.setBounds   (parameterLabelArea.removeFromRight(100));
    attackSlider.setBounds  (parameterArea.removeFromRight(100));
    releaseLabel.setBounds  (parameterLabelArea.removeFromRight(110));
    releaseSlider.setBounds (parameterArea.removeFromRight(100));
    
    signalStreamViewer.setBounds (172, 30, (getWidth() / 2) + 50, (getHeight() / 2) + 20) ;
}

void AmericanUniversityCompressorAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    repaint();
}
