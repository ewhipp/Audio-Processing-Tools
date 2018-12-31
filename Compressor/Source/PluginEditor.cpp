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
    typedef AmericanUniversityCompressorAudioProcessor::CompressorParameters compParams;
    tooltipWindow->setMillisecondsBeforeTipAppears (1000);
    
    attackLabel.setText(translate ("Attack"), dontSendNotification);
    addAndMakeVisible (attackLabel);
    addAndMakeVisible (attackSlider);
    attachments.add (new SliderAttachment (processor.getPluginState(), processor.getParameterId (compParams::ATTACK), attackSlider));
    attackSlider.setTooltip ("Attack time");
    
    releaseLabel.setText (translate ("Release"), dontSendNotification);
    addAndMakeVisible (releaseLabel);
    addAndMakeVisible (releaseSlider);
    attachments.add (new SliderAttachment (processor.getPluginState(), processor.getParameterId (compParams::RELEASE), releaseSlider));
    releaseSlider.setTooltip ("Release Time");
    
    ratioLabel.setText (translate ("Ratio"), dontSendNotification);
    addAndMakeVisible (ratioLabel);
    addAndMakeVisible (ratioSlider);
    attachments.add (new SliderAttachment (processor.getPluginState(), processor.getParameterId (compParams::RATIO), ratioSlider));
    ratioSlider.setTooltip ("Ratio of Compression");
    
    makeupGainLabel.setText(translate ("Make-up"), dontSendNotification);
    addAndMakeVisible (makeupGainLabel);
    addAndMakeVisible (makeupGainSlider);
    attachments.add (new SliderAttachment (processor.getPluginState(), processor.getParameterId (compParams::MAKEUPGAIN), makeupGainSlider));
    makeupGainSlider.setTooltip ("Make-up gain");
    
    thresholdLabel.setText(translate ("Threshold"), dontSendNotification);
    addAndMakeVisible(thresholdLabel);
    addAndMakeVisible(thresholdSlider);
    attachments.add (new SliderAttachment (processor.getPluginState(), processor.getParameterId (compParams::THRESHOLD), thresholdSlider));
    thresholdSlider.setTooltip ("Threshold of compression");

   // dBMeter.reset (new AudioMeter (2));
    
    signalStreamViewer.setNumChannels (2);
    signalStreamViewer.setColours (Colours::black, Colours::green);
    signalStreamViewer.setRepaintRate (30);
    addAndMakeVisible (signalStreamViewer);
    
    processor.addChangeListener (this);
    startTimerHz(30);
    setSize(580, 350);
}

AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
  //  dBMeter             = nullptr;
  //  engagementMeter     = nullptr;
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
  //  dBMeter->setVisualMeterLevel  (processor.getCurrentdB());
  //  dBMeterLabel.setText(Decibels::toString (processor.getCurrentdB()), dontSendNotification);
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
    
    ratioLabel.setBounds    (parameterLabelArea.removeFromRight(70));
    ratioSlider.setBounds   (parameterArea.removeFromRight(100));
    
    attackLabel.setBounds   (parameterLabelArea.removeFromRight(100));
    attackSlider.setBounds  (parameterArea.removeFromRight(100));
    
    releaseLabel.setBounds  (parameterLabelArea.removeFromRight(110));
    releaseSlider.setBounds (parameterArea.removeFromRight(100));
    
    // signalStreamViewer.setBounds (172, 30, (getWidth() / 2) + 50, (getHeight() / 2) + 20) ;
}

void AmericanUniversityCompressorAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    repaint();
}
