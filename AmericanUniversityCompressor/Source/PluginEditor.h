/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "AudioMeter.h"



//==============================================================================
/**
*/
class AmericanUniversityCompressorAudioProcessorEditor  : public AudioProcessorEditor,
                                                          public Slider::Listener,
                                                          private Timer
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    
    enum
    {
        parameterControlHeight = 40,
        paramaterControlLabelWidth = 80,
        parameterControlWidth = 300
    };
    
    AmericanUniversityCompressorAudioProcessorEditor (AmericanUniversityCompressorAudioProcessor&, AudioProcessorValueTreeState&);
    ~AmericanUniversityCompressorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider* slider) override;
    void sliderDragEnded (Slider* slider) override;
    void timerCallback() override;
    
private:
    // This reference is provided as a quick way for your editor to access the processor object
    // that created it.
    AmericanUniversityCompressorAudioProcessor& processor;
    
    // Easly retrieve the parameters for the sliders
    AudioParameterFloat* getParameterForSlider(Slider* slider)
    {
        const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
        return dynamic_cast<AudioParameterFloat*> (params[paramSliders.indexOf(slider)]);
    }
    
    // Parameters
    AudioProcessorValueTreeState& valueTreeState;
    AudioProcessorValueTreeState& valueTreeState;
    
    OwnedArray<Slider> paramSliders;
    OwnedArray<Label> paramLabels;
    Label ratioLabel;
    Slider ratioSlider;
    ScopedPointer<SliderAttachment> ratioAttachment;
   
    AudioMeter rmsValue;
    Label rmsValueLabel;
    
    AudioMeter rms2DBValue;
    Label rms2DBValueLabel;
    Label currentGainEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmericanUniversityCompressorAudioProcessorEditor)
};
