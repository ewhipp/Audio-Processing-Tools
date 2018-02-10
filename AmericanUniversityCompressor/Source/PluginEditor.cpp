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

    // Make meters visible
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


 // This function handles the acquisition of the slider values/ the changes.
void AmericanUniversityCompressorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider(slider))
        *param = (float) slider->getValue();
}

 // This function responds to the handling of moving a slider value
void AmericanUniversityCompressorAudioProcessorEditor::sliderDragStarted(Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider(slider))
        param->beginChangeGesture();
}

// This function handles the end of handling for a slider value
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
    g.setFont (12.0f);
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
}

