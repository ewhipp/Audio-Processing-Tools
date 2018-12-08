/*
  ==============================================================================

    OpenGLAudioVisualizeBuffer.h
    Created: 4 Nov 2018 5:59:18pm
    Author:  Erik Whipp

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#pragma once
class OpenGLAudioVisualiserComponent  : public Component,
                                        public OpenGLRenderer,
                                        public Timer
{
public:
    /** Creates a visualiser with the given number of channels. */
    OpenGLAudioVisualiserComponent (int initialNumChannels);
    
    /** Destructor. */
    ~OpenGLAudioVisualiserComponent();
    
    /** Changes the number of channels that the visualiser stores. */
    void setNumChannels (int numChannels);
    
    /** Changes the number of samples that the visualiser keeps in its history.
     Note that this value refers to the number of averaged sample blocks, and each
     block is calculated as the peak of a number of incoming audio samples. To set
     the number of incoming samples per block, use setSamplesPerBlock().
    */
    void setBufferSize (int bufferSize);
    
    /** */
    void setSamplesPerBlock (int newNumInputSamplesPerBlock) noexcept;
    
    /** */
    int getSamplesPerBlock() const noexcept   { return inputSamplesPerBlock; }
    
    /** Clears the contents of the buffers. */
    void clear();
    
    /** Pushes a buffer of channels data.
     The number of channels provided here is expected to match the number of channels
     that this AudioVisualiserComponent has been told to use.
     */
    void pushBuffer (const AudioSampleBuffer& bufferToPush);
    
    /** Pushes a buffer of channels data.
     The number of channels provided here is expected to match the number of channels
     that this AudioVisualiserComponent has been told to use.
     */
    void pushBuffer (const AudioSourceChannelInfo& bufferToPush);
    
    /** Pushes a buffer of channels data.
     The number of channels provided here is expected to match the number of channels
     that this AudioVisualiserComponent has been told to use.
     */
    void pushBuffer (const float** channelData, int numChannels, int numSamples);
    
    /** Pushes a single sample (per channel).
     The number of channels provided here is expected to match the number of channels
     that this AudioVisualiserComponent has been told to use.
     */
    void pushSample (const float* samplesForEachChannel, int numChannels);
    
    /** Sets the colours used to paint the */
    void setColours (Colour backgroundColour, Colour waveformColour) noexcept;
    
    /** Sets the frequency at which the component repaints itself. */
    void setRepaintRate (int frequencyInHz);
    
    //==============================================================================
    void paint (Graphics&) override;
    void newOpenGLContextCreated() override;
    void openGLContextClosing() override;
    void renderOpenGL() override;
    
private:
    
    OpenGLContext openGLContext;
    
    struct ChannelInfo;
    friend struct ChannelInfo;
    friend struct ContainerDeletePolicy<ChannelInfo>;
    
    OwnedArray<ChannelInfo> channels;
    int numSamples, inputSamplesPerBlock;
    Colour backgroundColour, waveformColour;
    
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLAudioVisualiserComponent)
};
