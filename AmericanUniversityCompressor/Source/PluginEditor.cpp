/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
AmericanUniversityCompressorAudioProcessorEditor::AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor& parent)
: AudioProcessorEditor (&parent), processor (parent)
{
    // Get audio processor parameters
    const OwnedArray<AudioProcessorParameter>& params = parent.getParameters();
    
    // Attach components to audio processor GUI elements
    for(int i = 0; i < params.size(); ++i)
    {
        if(const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*>(params[i]))
        {
            Slider* genericSlider;
            paramSliders.add(genericSlider = new Slider(param->name));
            genericSlider->setRange (param->range.start, param->range.end);
            genericSlider->setSliderStyle(Slider::LinearHorizontal);
            genericSlider->setValue(*param);
            
            genericSlider->addListener(this);
            addAndMakeVisible(genericSlider);
            
            Label* genericLabel;
            paramLabels.add(genericLabel = new Label(param->name, param->name));
            addAndMakeVisible(genericLabel);
        }
    }
    
    startTimerHz(30);
    
    setSize (580, 350);

    /*
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
    */
    addAndMakeVisible(rmsValue);
    
    addAndMakeVisible(rmsValueLabel);
    rmsValueLabel.setText("RMS", dontSendNotification);
    rmsValueLabel.attachToComponent(&rmsValue, false);
    
    
    addAndMakeVisible(rms2DBValue);
    
    addAndMakeVisible(rms2DBValueLabel);
    rms2DBValueLabel.setText("dB", dontSendNotification);
    rms2DBValueLabel.attachToComponent(&rms2DBValue, false);
}

// Destructor
AmericanUniversityCompressorAudioProcessorEditor::~AmericanUniversityCompressorAudioProcessorEditor()
{
}

/*
 * This function handles the acquisition of the slider values/ the changes.
 */
void AmericanUniversityCompressorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider(slider))
        *param = (float) slider->getValue();
}

/*
 * This function responds to the handling of moving a slider value
 */
void AmericanUniversityCompressorAudioProcessorEditor::sliderDragStarted(Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider(slider))
        param->beginChangeGesture();
}

/*
 * This function handles the end of handling for a slider value
 */
void AmericanUniversityCompressorAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider(slider))
        param->endChangeGesture();
}

// Repaint the meters. For more info on meter creation see AudioMeter.h
void AmericanUniversityCompressorAudioProcessorEditor::timerCallback()
{
    // Initalize and retrieve parameters from the audio processor
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    for (int i = 0; i < params.size(); ++i)
    {
        if(const AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*>(params[i]))
        {
            if (i < paramSliders.size())
                paramSliders[i]->setValue(*param);
        }
}
    
    
    // Update rms meter
    rmsValue.setLevelRMS(processor.currentRMS);
    rmsValueLabel.setText(std::to_string(processor.currentRMS),
                          dontSendNotification);

    // Update the dB meter
    rms2DBValue.setLeveldB(processor.currentdB);
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
    Rectangle<int> pluginWindow = getLocalBounds();
    auto someSpace = pluginWindow.removeFromTop(145);
    auto MeterArea = pluginWindow.removeFromLeft(200);
    auto LabelArea = MeterArea.removeFromTop(25);
   
    // Slider parameters
    for(int i = 0; i <paramSliders.size(); ++i)
    {
        Rectangle<int> parameterBounds = pluginWindow.removeFromTop(parameterControlHeight);
        Rectangle<int> labelBounds = parameterBounds.removeFromLeft(paramaterControlLabelWidth);

        paramLabels[i]->setBounds (labelBounds);
        paramSliders[i]->setBounds (parameterBounds);
    }
    
    // Meters
    rmsValue.setBounds(MeterArea.removeFromLeft(100));
    rmsValueLabel.setBounds(LabelArea.removeFromLeft(100));
    rms2DBValue.setBounds(MeterArea.removeFromLeft(100));
    rms2DBValueLabel.setBounds(LabelArea.removeFromLeft(100));
    
    /*
     releaseTimeLabel.setBounds(labelArea.removeFromLeft(80))
     releaseTime.setBounds(SliderArea.removeFromLeft(80));
     
     makeupGainLabel.setBounds(labelArea.removeFromLeft(80));
     makeupGain.setBounds(SliderArea.removeFromLeft(80));
     
     thresholdLabel.setBounds(labelArea.removeFromLeft(80));
     threshold.setBounds(SliderArea.removeFromLeft(80));
     
     ratioLabel.setBounds(labelArea.removeFromLeft(80));
     ratio.setBounds(SliderArea.removeFromLeft(80));
     
     attackTimeLabel.setBounds(labelArea.removeFromLeft(80));
     attackTime.setBounds(SliderArea.removeFromLeft(80));;*/
}
