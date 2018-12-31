/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CompressorProcessor.h"

//==============================================================================
/**
*/
class CompressorAudioProcessor  :                   public AudioProcessor,
                                                    public AudioProcessorValueTreeState::Listener,
                                                    public ChangeBroadcaster
{
public:
    //==============================================================================
    enum CompressorParameters
    {
        ATTACK = 0,
        RELEASE,
        THRESHOLD,
        MAKEUPGAIN,
        RATIO,
        KNEE,
        MAX_COMP_PARAMS
    };
    
    //==============================================================================
    CompressorAudioProcessor();
    ~CompressorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void parameterChanged (const String& parameter, float newValue) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AudioProcessorValueTreeState& getPluginState();
    
    //==============================================================================
    float getCurrentdB();
    float getCurrentGainFactor();
    float getCurrentThresholdRMS();
    float getCurrentRMS();
    float getTargetGainFactor();
    float getCurrentOvershoot();

    //==============================================================================
    AudioSampleBuffer getVisualBuffer();
    int getVisualBufferChannels();

    //==============================================================================
    static String getParameterId (size_t);
    float* getParameterValue (String);
    
private:
    AudioProcessorValueTreeState state;
    UndoManager                  undo;
    
    float m_lastOvershoot;
    float m_currentOvershoot;
    float m_blockTargetGainFactor;
    float m_currentGainFactor;
    float m_currentRMS;
    float m_thresholdRMS;
    float m_currentdB;
    
    std::unique_ptr<CompressorProcessor> m_compressor;
    
    bool attackFlag;
    
    AudioSampleBuffer visualizeBuffer;
    
    /**
     * Moving RMS function.
     *
     * @param n: number to finish window
     * @param buffer: current buffer amplitude amount.
     */
    float ampToRMS (int n, const float *buffer)
    {
        float total = 0.0;
        
        for (int i = 0; i < n; i++)
            total += powf(buffer[i], 2.0);
        
        total /= n;
        total = sqrt(total);
        return total;
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessor)
};
