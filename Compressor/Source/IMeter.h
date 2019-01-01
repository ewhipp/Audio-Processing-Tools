/*
  ==============================================================================

    IMeter.h
    Created: 30 Dec 2018 12:49:09am
    Author:  Erik Whipp

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include <exception>
#include <string>

#ifndef IMETER_H
#define IMETER_H

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
    
private:
};


/**
 *  Interface for all meters.
 */
class IMeter :          public Component
{  
protected:
    float m_incomingSignal;
    float m_minimumValue;
    float m_maximumValue;
    
public:
    virtual void paint                   (Graphics&) = 0;
    
    virtual const void setMinimumValue       (float) = 0;
    virtual const void setMaximumValue       (float) = 0;
    
    virtual void setIncomingSignal           (float) = 0;
    virtual float normalize                  (float) = 0;
    
    virtual void const setType               (int)   = 0;
    
    enum METER_TYPE
    {
        LEVEL = 0,
        ENGAGEMENT,
        RMS,
        MAX_METER_TYPES
    };
    
private:
};

#endif
