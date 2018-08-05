/*
  ==============================================================================

    VisualizationView.h
    Created: 5 Aug 2018 2:05:24am
    Author:  Erik Whipp

    A component to view audio sample buffers utilizing OpenGL instead of generic
    software rendering.
  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class VisualizationView : public Component,
                          public OpenGLRenderer,
                          public Timer
{
public:
    VisualizationView (const int /* initialNumChannels */, int /* blockSize */,
                      Colour /* backgroundColour */, Colour /* waveformColour */);
    ~VisualizationView();
    
    /**
     * Clear the current visual buffer of all content.
     */
    void clear();
    
    /**
     * Push an entire AudioSampleBuffer into the visual buffer context.
     * This will push all information and expects the same number of channels
     * as the current VisualizationView.
     *
     * @param bufferToPush: The buffer to pull from.
     */
    void pushNewBuffer (const AudioSampleBuffer& /* bufferToPush */);
    
    /**
     * Copy an entire channel's buffer into the visual buffer content.
     * This will copy all information and expects the same number of channels
     * as the current VisualizationView.
     *
     * @param channelToCopy: The channel to copy from.
     */
    void copyBuffer (const AudioSourceChannelInfo& /* channelToCopy */);
    
    /**
     * Push current channel data into the visual buffer content.
     * This will (like a moving window) push information into the VisualizationView.
     *
     * @param channelData: Pointer to the current sample.
     * @param numChannels: The number of channels the data holds.
     * @param numSamples: The number of samples to push into the VisualizationView.
     */
    void pushChannelData (const float** /* channelData */, int /* numChannels */, int /* numSamples */);
    
    /**
     * Push a singular sample into the VisualizationView.
     *
     * @param sample: Pointer to a specific sample
     * @param numChannels: Number of channels a sample has information from.
     */
    void pushSingleSample (const float* /* sample */, int /* numChannels */);
    
    /* Setters */
    void setRepaintRate (int /* Hz */);
    void setNumChannels (const int /* numChannels */);
    void setBufferSize (int /* bufferSize */);
    void setSamplesPerBlock (int /* samplesRate */) noexcept;
    void setColor (Colour /* backgroundColor */, Colour /* waveformColor */) noexcept;

    /* Inherited */
    void paint (Graphics&) override;
    void newOpenGLContextCreated() override;
    void openGLContextClosing() override;
    void renderOpenGL() override;
    
private:
    
    OpenGLContext context;
    OwnedArray<ChannelInfo> channels;
};

