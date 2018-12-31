/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& parent)
:   AudioProcessorEditor (&parent),
    m_processor        (parent),
    m_makeupGainSlider (Slider::LinearVertical, Slider::TextBoxBelow),
    m_thresholdSlider  (Slider::LinearVertical, Slider::TextBoxBelow),
    m_attackSlider     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    m_releaseSlider    (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    m_ratioSlider      (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    m_signalStreamViewer (m_processor.getTotalNumInputChannels())
{
    typedef CompressorAudioProcessor::CompressorParameters compParams;
    m_tooltipWindow->setMillisecondsBeforeTipAppears (1000);
    
    m_attackLabel.setText(translate ("Attack"), dontSendNotification);
    addAndMakeVisible (attackLabel);
    addAndMakeVisible (attackSlider);
    m_attachments.add (new SliderAttachment (m_processor.getPluginState(), m_processor.getParameterId (compParams::ATTACK), m_attackSlider));
    m_attackSlider.setTooltip ("Attack time");
    
    m_releaseLabel.setText (translate ("Release"), dontSendNotification);
    addAndMakeVisible (releaseLabel);
    addAndMakeVisible (releaseSlider);
    m_attachments.add (new SliderAttachment (m_processor.getPluginState(), m_processor.getParameterId (compParams::RELEASE), m_releaseSlider));
    m_releaseSlider.setTooltip ("Release Time");
    
    m_ratioLabel.setText (translate ("Ratio"), dontSendNotification);
    addAndMakeVisible (ratioLabel);
    addAndMakeVisible (ratioSlider);
    m_attachments.add (new SliderAttachment (m_processor.getPluginState(), m_processor.getParameterId (compParams::RATIO), m_ratioSlider));
    m_ratioSlider.setTooltip ("Ratio of Compression");
    
    m_makeupGainLabel.setText(translate ("Make-up"), dontSendNotification);
    addAndMakeVisible (makeupGainLabel);
    addAndMakeVisible (makeupGainSlider);
    m_attachments.add (new SliderAttachment (m_processor.getPluginState(), m_processor.getParameterId (compParams::MAKEUPGAIN), m_makeupGainSlider));
    m_makeupGainSlider.setTooltip ("Make-up gain");
    
    m_thresholdLabel.setText(translate ("Threshold"), dontSendNotification);
    addAndMakeVisible(thresholdLabel);
    addAndMakeVisible(thresholdSlider);
    m_attachments.add (new SliderAttachment (m_processor.getPluginState(), m_processor.getParameterId (compParams::THRESHOLD), m_thresholdSlider));
    m_thresholdSlider.setTooltip ("Threshold of compression");

   // dBMeter.reset (new AudioMeter (2));
    
    m_signalStreamViewer.setNumChannels (2);
    m_signalStreamViewer.setColours (Colours::black, Colours::green);
    m_signalStreamViewer.setRepaintRate (30);
    addAndMakeVisible (m_signalStreamViewer);
    
    processor.addChangeListener (this);
    startTimerHz(30);
    setSize(580, 350);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
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
 * @see: Meter
 */
void CompressorAudioProcessorEditor::timerCallback()
{
    m_signalStreamViewer.pushBuffer (m_processor.getVisualBuffer());
  //  dBMeter->setVisualMeterLevel  (processor.getCurrentdB());
  //  dBMeterLabel.setText(Decibels::toString (processor.getCurrentdB()), dontSendNotification);
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (12.0f);
}

void CompressorAudioProcessorEditor::resized()
{
    
    Rectangle<int> pluginWindow = getLocalBounds();
    Rectangle<int> sliderLabelArea = pluginWindow.removeFromTop(50);
    
    m_thresholdLabel.setBounds   (sliderLabelArea.removeFromLeft(100));
    m_thresholdSlider.setBounds  (pluginWindow.removeFromLeft(100));
    
    m_makeupGainLabel.setBounds  (sliderLabelArea.removeFromRight(70));
    m_makeupGainSlider.setBounds (pluginWindow.removeFromRight(80));
    
    Rectangle<int> parameterArea =      pluginWindow.removeFromBottom(100);
    Rectangle<int> parameterLabelArea = pluginWindow.removeFromBottom(25);
    
    m_ratioLabel.setBounds    (parameterLabelArea.removeFromRight(70));
    m_ratioSlider.setBounds   (parameterArea.removeFromRight(100));
    
    m_attackLabel.setBounds   (parameterLabelArea.removeFromRight(100));
    m_attackSlider.setBounds  (parameterArea.removeFromRight(100));
    
    m_releaseLabel.setBounds  (parameterLabelArea.removeFromRight(110));
    m_releaseSlider.setBounds (parameterArea.removeFromRight(100));
    
    // signalStreamViewer.setBounds (172, 30, (getWidth() / 2) + 50, (getHeight() / 2) + 20) ;
}

void CompressorAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    repaint();
}
