/*
  ==============================================================================

    Meter.h
    Created: 30 Dec 2018 12:57:35am
    Author:  Erik Whipp

    Base class for visual meters.
  ==============================================================================
*/

#include "IMeter.h"
#include <exception>
#include <string>

#ifndef METER_H
#define METER_H

/**
 *  An exception for initialization of a meter. Generally, this exception should
 *  only be thrown if the type is not set in the constructor of the meter.
 *
 *  @param m_message: Message that will be sent upon catching the exception.
 */
class MeterInitializationException     : public std::runtime_error
{
protected:
    std::string m_message = "";
    
public:
    explicit MeterInitializationException (const std::string& message): std::runtime_error (message) { }
    explicit MeterInitializationException (const char* message) : std::runtime_error (message) { }
    virtual ~MeterInitializationException() throw() { }
};

/**
 * Base meter class for all types of meters. Future implementations should
 * convert enum types of meter to a specific derived class. Defaults to a
 * simple audio meter that displays level from -100.0 dB to 0.0dB
 *
 * @param m_type: METER_TYPE decides the meter to display. Default : Audio.
 */
class Meter :           public IMeter
{
public:
    enum METER_TYPE
    {
        AUDIO = 0,
        VISUAL,
        ENGAGEMENT,
        RMS,
        MAX_METER_TYPES
    };

    Meter (METER_TYPE type): m_type (type)
    {
        try
        {
            setType (m_type);
            
            // This is where we need to define the maximum & minimum values for the slider
            if (m_type != METER_TYPE::AUDIO)
            {
                reconstructMeter (m_type);
            }
        }
        catch (MeterInitializationException)
        {
            std::cerr << "ERROR: Meter must be initialized upon creation.\n";
        }
        
        setPaintingIsUnclipped (true);
    }
    ~Meter() { }
    
    void paint (Graphics&) override;
    
    virtual float normalize (float) override;
    virtual void setValue   (float) override;
    
    virtual const void setMinimumValue (float) override;
    virtual const void setMaximumValue (float) override;
    
    virtual const void setType (int) noexcept final;
    const Meter reconstructMeter (METER_TYPE& type);

private:
    METER_TYPE m_type;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)
};

#endif
