/*
  ==============================================================================

    RMSMeter.h
    Created: 30 Dec 2018 1:48:24am
    Author:  Erik Whipp

  ==============================================================================
*/


#ifndef RMSMETER_H
#define RMSMETER_H

#include "IMeter.h"

class RMSMeter : public IMeter
{
public:
    RMSMeter (METER_TYPE type = METER_TYPE::RMS, float maximumValue = 1.0f, float minimumValue = 0.0f)
    : m_type (type)
    {
        try
        {
            setType (type);
        }
        catch (MeterInitializationException)
        {
            std::cerr << "Error initializing RMS Meter";
        }
        
        setMaximumValue (maximumValue);
        setMinimumValue (minimumValue);
    }
    
    ~RMSMeter() { }
    
    void paint (Graphics&) override;
    
    virtual float normalize (float) override;
    virtual void setValue   (float) override;
    
    virtual const void setMinimumValue (float) override;
    virtual const void setMaximumValue (float) override;
    
    virtual const void setType (int) noexcept final;

        
private:
    METER_TYPE m_type;
};

#endif
