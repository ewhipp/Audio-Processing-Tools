/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
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
   
    
    setSize (400, 150);
}

AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
}

/*
 * This function handles the acquisition of the slider values/ the changes.
 */
void AmericanUniversityCompressorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{/*
    // Make the ratio come out as a real ratio (1:8) etc.
  if(slider == &ratio)
  {
      if (ratio.getValue() == 0.125)
      {
          ratio.setTextValueSuffix("");
          ratio.setTextValueSuffix("1:8");
      }
      else if (ratio.getValue() == 0.250)
      {
          ratio.setTextValueSuffix("1:4");
      }
      else if (ratio.getValue() == 0.375)
      {
          ratio.setTextValueSuffix("3:8");
      }
      else if (ratio.getValue() == 0.5)
      {
          ratio.setTextValueSuffix("1:2");
      }
      else if (ratio.getValue() == 0.625)
      {
          ratio.setTextValueSuffix("5:8");
      }
      else if (ratio.getValue() == 0.750)
      {
          ratio.setTextValueSuffix("3:4");
      }
      else if (ratio.getValue() == 0.875)
      {
          ratio.setTextValueSuffix("7:8");
      }
      else
      {
          ratio.setTextValueSuffix("1:1");
      }
  }*/
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
    auto SliderArea = pluginWindow.removeFromBottom(100);
    auto labelArea = pluginWindow.removeFromBottom(100);
   
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
    
}
