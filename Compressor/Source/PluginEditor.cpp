/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
AmericanUniversityCompressorAudioProcessorEditor::AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor& parent, AudioProcessorValueTreeState &vts)
:   AudioProcessorEditor (&parent), processor (parent), valueTreeState(vts), audioView(processor.getVisualBufferChannels())
{
    // Labels & Slider init
    // rotary sliders
    attackLabel.setText("Attack", dontSendNotification);
    addAndMakeVisible(attackLabel);
    attackSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 3);
    addAndMakeVisible (attackSlider);
    attackAttachment = new SliderAttachment (valueTreeState, "attack", *attackSlider);
    
    releaseLabel.setText ("Release", dontSendNotification);
    addAndMakeVisible (releaseLabel);
    releaseSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 3);
    addAndMakeVisible(releaseSlider);
    releaseAttachment = new SliderAttachment (valueTreeState, "release", *releaseSlider);
    
    ratioLabel.setText ("Ratio", dontSendNotification);
    addAndMakeVisible (ratioLabel);
    ratioSlider = new TextFormatSlider (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow, 4);
    addAndMakeVisible (ratioSlider);
    ratioAttachment = new SliderAttachment (valueTreeState, "ratio", *ratioSlider);
    
    makeupGainLabel.setText("Make-up", dontSendNotification);
    addAndMakeVisible(makeupGainLabel);
    makeupGainSlider = new TextFormatSlider (Slider::LinearVertical, Slider::TextBoxBelow, 1);
    addAndMakeVisible(makeupGainSlider);
    makeupAttachment = new SliderAttachment (valueTreeState, "makeUpGain", *makeupGainSlider);
    
    thresholdLabel.setText("Threshold", dontSendNotification);
    addAndMakeVisible(thresholdLabel);
    thresholdSlider = new TextFormatSlider (Slider::LinearVertical, Slider::TextBoxBelow, 1);
    addAndMakeVisible(thresholdSlider);
    thresholdAttachment = new SliderAttachment (valueTreeState, "threshold", *thresholdSlider);
    
    rms2DBValue = new AudioMeter (2);
    
    compressorEngagementVisualizer = new EngagementMeter(100, 100);
    
    
    audioView.setNumChannels (2);
    audioView.setColours (Colours::black, Colours::yellowgreen);
    audioView.setRepaintRate (30);
    addAndMakeVisible (audioView);
    
    // Meters and debugging
    addAndMakeVisible(rmsValue);
    addAndMakeVisible(rmsValueLabel);
    addAndMakeVisible(rms2DBValue);
    addAndMakeVisible(rms2DBValueLabel);
    processor.addChangeListener (this);
    startTimerHz(30);
    setSize(580, 350);
}

AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
    thresholdAttachment             = nullptr;
    makeupAttachment                = nullptr;
    ratioAttachment                 = nullptr;
    attackAttachment                = nullptr;
    releaseAttachment               = nullptr;
    ratioSlider                     = nullptr;
    attackSlider                    = nullptr;
    releaseSlider                   = nullptr;
    thresholdSlider                 = nullptr;
    makeupGainSlider                = nullptr;
    rms2DBValue                     = nullptr;
    compressorEngagementVisualizer  = nullptr;
}

/*
 * Repaint visual fields.
 *
 * @see: AudioMeter, EngagementMeter
 */
void AmericanUniversityCompressorAudioProcessorEditor::timerCallback()
{
    // Update the visualization buffer.
    audioView.pushBuffer(processor.getVisualBuffer()); // Copy of the processBlock buffer
    
    // Update the dB meter
    rms2DBValue->setVisualMeterLevel(processor.getCurrentdB());
    rms2DBValueLabel.setText(Decibels::toString(processor.getCurrentdB()),
                             dontSendNotification);
    
    // Update EngagementMeter
    compressorEngagementVisualizer->moveMeter(processor.getTargetGainFactor());
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
    currentGainEditor.setBounds(240, 50, 25, 300);
    currentThresholdRMS.setBounds(190, 50, 100, 100);
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

    MeterArea.removeFromLeft(17);
    rms2DBValue->setBounds(MeterArea.removeFromLeft(20));
    
    MeterArea.removeFromLeft(17);
    compressorEngagementVisualizer->setBounds(MeterArea.removeFromLeft(50));
    // rms2DBValueLabel.setBounds(LabelArea.removeFromLeft(70)); If we ultimately want to see the level value
    
    audioView.setBounds(172, 30, (getWidth() / 2) + 50, (getHeight() / 2) + 20) ;
}

void AmericanUniversityCompressorAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused(sender);
    repaint();
}
