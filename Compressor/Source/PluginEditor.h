/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Meter.h"

//==============================================================================
class CompressorAudioProcessorEditor  :                   public AudioProcessorEditor,
                                                          public ChangeListener,
                                                          private Timer
{
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    CompressorAudioProcessorEditor (CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor();

    //==============================================================================
    
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void changeListenerCallback(ChangeBroadcaster* sender) override;
    
private:
    CompressorAudioProcessor& m_processor;
    
    Label           m_makeupGainLabel;
    Slider      	m_makeupGainSlider;
    
    Label           m_thresholdLabel;
    Slider          m_thresholdSlider;
    
    Label       	m_attackLabel;
    Slider      	m_attackSlider;
    
    Label       	m_releaseLabel;
    Slider          m_releaseSlider;
    
    Label           m_ratioLabel;
    Slider          m_ratioSlider;
    
    ToggleButton    m_kneeButton;
    
    OwnedArray <SliderAttachment> m_sliderAttachments;
    OwnedArray <ButtonAttachment> m_buttonAttachments;
    SharedResourcePointer <TooltipWindow> m_tooltipWindow;
        
    AudioVisualiserComponent m_signalStreamViewer;
    
    std::unique_ptr <Meter> m_levelMeter;
    std::unique_ptr <Meter> m_engagementMeter;
    
    std::unique_ptr <DropShadower> m_levelShadower;
    std::unique_ptr <DropShadower> m_engagementShadower;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};
