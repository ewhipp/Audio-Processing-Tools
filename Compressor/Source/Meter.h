/*
  ==============================================================================

    Meter.h
    Created: 31 Dec 2018 10:43:10am
    Author:  Erik Whipp

  ==============================================================================
*/

#include "IMeter.h"

#ifndef METER_H
#define METER_H

class Meter :           public IMeter
{
public:
    

    Meter (METER_TYPE type, float minimumValue = 0.0f, float maximumValue = 1.0f)
        : m_type (type)
    {
        DBG ("Initializing Meter");
        
        try
        {
            setType (m_type);
            setMinimumValue (minimumValue);
            setMaximumValue (maximumValue);
        }
        catch (MeterInitializationException)
        {
            std::cerr << "ERROR: Meter must be initialized upon creation.\n";
        }
        
        setPaintingIsUnclipped (true);
    }
    ~Meter() { }
    // ==============================================================================
    void paint (Graphics& /* g */) override;
    
    /**
     * A basic meter is similar to the meters in a standard DAW that show you how
     * much signal is passing through a specific channel. Based on the incoming signal,
     * a black rectangle will fill with red color to showcase the amount of signal passing
     * through the meter.
     *
     * @param g: The graphics context
     */
    void drawBasicMeter (Graphics& /* g */);
    
    /**
     * An engagement meter is similar to the meter in a standard compressor that shows how
     * much the compressor is compressor. It is generally half an elipsis with an arrow
     * that moves from one side to the other based on the RMS value that is passing through
     * the specified channel.
     *
     * @param g: The graphics context
     */
    void drawEngagement (Graphics& /* g */);
    void drawVisual (Graphics& /* g */);
    
    // ==============================================================================
    virtual float normalize (float /* incomingSignal */) override;
    float normalize (float /* incomingSignal */, float /* maximumValue */, float /* minimumValue */);
    virtual void setIncomingSignal   (float /* value */) override;
    virtual const void setMinimumValue (float /* value */) override;
    virtual const void setMaximumValue (float /* value */) override;
    virtual const void setType (int /* type */) noexcept final;
    
private:
    METER_TYPE m_type;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)
};

#endif
