/*
  ==============================================================================

    OpenGLAudioVisualizeBuffer.cpp
    Created: 4 Nov 2018 5:59:18pm
    Author:  Erik Whipp

  ==============================================================================
*/

#include "OpenGLAudioVisualiser.h"
#include "OpenGL/gl.h"

struct OpenGLAudioVisualiserComponent::ChannelInfo
{
    ChannelInfo (OpenGLAudioVisualiserComponent& o, int bufferSize)
    : owner (o), nextSample (0), subSample (0)
    {
        setBufferSize (bufferSize);
        clear();
    }
    
    void clear() noexcept
    {
        for (int i = 0; i < levels.size(); ++i)
            levels.getReference(i) = Range<float>();
        
        value = Range<float>();
        subSample = 0;
    }
    
    void pushSamples (const float* inputSamples, const int num) noexcept
    {
        for (int i = 0; i < num; ++i)
            pushSample (inputSamples[i]);
    }
    
    void pushSample (const float newSample) noexcept
    {
        if (--subSample <= 0)
        {
            nextSample %= levels.size();
            levels.getReference (nextSample++) = value;
            subSample = owner.getSamplesPerBlock();
            value = Range<float> (newSample, newSample);
        }
        else
        {
            value = value.getUnionWith (newSample);
        }
    }
    
    void setBufferSize (int newSize)
    {
        levels.removeRange (newSize, levels.size());
        levels.insertMultiple (-1, Range<float>(), newSize - levels.size());
        
        if (nextSample >= newSize)
            nextSample = 0;
    }
    
    OpenGLAudioVisualiserComponent& owner;
    Array<Range<float>> levels;
    Range<float> value;
    int nextSample, subSample;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelInfo)
};

//==============================================================================
OpenGLAudioVisualiserComponent::OpenGLAudioVisualiserComponent (const int initialNumChannels)
: numSamples (1024),
inputSamplesPerBlock (256),
backgroundColour (Colours::black),
waveformColour (Colours::white)
{
    setOpaque (true);
    setNumChannels (initialNumChannels);
    setRepaintRate (60);
    
    openGLContext.setComponentPaintingEnabled(true);
    openGLContext.setRenderer(this);
    openGLContext.attachTo(*this);
}

OpenGLAudioVisualiserComponent::~OpenGLAudioVisualiserComponent()
{
    DBG ("SHUTTING DOWN!");
    openGLContext.detach();
    openGLContext.deactivateCurrentContext();
}

void OpenGLAudioVisualiserComponent::setNumChannels (const int numChannels)
{
    channels.clear();
    
    for (int i = 0; i < numChannels; ++i)
        channels.add (new ChannelInfo (*this, numSamples));
}

void OpenGLAudioVisualiserComponent::setBufferSize (int newNumSamples)
{
    numSamples = newNumSamples;
    
    for (int i = 0; i < channels.size(); ++i)
        channels.getUnchecked(i)->setBufferSize (newNumSamples);
}

void OpenGLAudioVisualiserComponent::clear()
{
    for (int i = 0; i < channels.size(); ++i)
        channels.getUnchecked(i)->clear();
}

void OpenGLAudioVisualiserComponent::pushBuffer (const float** d, int numChannels, int num)
{
    numChannels = jmin (numChannels, channels.size());
    
    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (d[i], num);
}

void OpenGLAudioVisualiserComponent::pushBuffer (const AudioSampleBuffer& buffer)
{
    pushBuffer (buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

void OpenGLAudioVisualiserComponent::pushBuffer (const AudioSourceChannelInfo& buffer)
{
    const int numChannels = jmin (buffer.buffer->getNumChannels(), channels.size());
    
    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (buffer.buffer->getReadPointer (i, buffer.startSample),
                                               buffer.numSamples);
}

void OpenGLAudioVisualiserComponent::pushSample (const float* d, int numChannels)
{
    numChannels = jmin (numChannels, channels.size());
    
    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSample (d[i]);
}

void OpenGLAudioVisualiserComponent::setSamplesPerBlock (int newSamplesPerPixel) noexcept
{
    inputSamplesPerBlock = newSamplesPerPixel;
}

void OpenGLAudioVisualiserComponent::setRepaintRate (int frequencyInHz)
{
    startTimerHz (frequencyInHz);
}

void OpenGLAudioVisualiserComponent::timerCallback()
{
    repaint();
}

void OpenGLAudioVisualiserComponent::setColours (Colour bk, Colour fg) noexcept
{
    backgroundColour = bk;
    waveformColour = fg;
    renderOpenGL();
}

void OpenGLAudioVisualiserComponent::paint (Graphics& g)
{
    // All Happens in OpenGL
}

void OpenGLAudioVisualiserComponent::newOpenGLContextCreated() { }
void OpenGLAudioVisualiserComponent::openGLContextClosing() {
    openGLContext.deactivateCurrentContext();
    openGLContext.detach();
}

void OpenGLAudioVisualiserComponent::renderOpenGL()
{
    const float desktopScale = (float) openGLContext.getRenderingScale();
    OpenGLHelpers::clear (backgroundColour);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport (0, 0, roundToInt (desktopScale * getWidth()), roundToInt (desktopScale * getHeight()));
    
    glLoadIdentity();
    glTranslated(0.0, 0.0, 0.0);
    
    glBegin(GL_LINE_STRIP); // render the waveform display
    
    glColor3f(waveformColour.getFloatRed(), waveformColour.getFloatGreen(), waveformColour.getFloatBlue());
    
    for (int i = 0; i < channels.size(); ++i)
    {
        const ChannelInfo& c = *channels.getUnchecked(i);
        
        int numLevels = c.levels.size();
        int nextSample = c.nextSample;
        Range<float>* levels = c.levels.begin();
        
        for (int i = 0; i < numLevels; ++i)
        {
            const float level = -(levels[(nextSample + i) % numLevels].getEnd());
            float x = ((2.0 / numLevels) * i) - 1;
            
            if (i == 0)
                glVertex3f(x, 0, 0);
            else
                glVertex3f((float) x, level, 0);
        }
        
        for (int i = numLevels; --i >= 0;) {
            float x = ((2.0 / numLevels) * i) - 1;
            glVertex3f((float) x, -(levels[(nextSample + i) % numLevels].getStart()), 0);
        }
    }
    
    glEnd();
}
