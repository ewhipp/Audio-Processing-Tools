/*
  ==============================================================================

    RMSMeter.h
    Created: 30 Dec 2018 1:48:24am
    Author:  Erik Whipp

  ==============================================================================
*/


#ifndef RMSMETER_H
#define RMSMETER_H

#include "Meter.h"

class RMSMeter : public Meter
{
public:
    RMSMeter (METER_TYPE type = METER_TYPE::RMS, float maximumValue = 1.0f, float minimumValue = 0.0f)
    : Meter (type)
    {
        setMaximumValue (maximumValue);
        setMinimumValue (minimumValue);
    }
    
    ~RMSMeter() { }
    
    void paint (Graphics&) override;
    
    virtual float normalize (float) override;
    virtual void setValue   (float) override;
    
    virtual const void setMinimumValue (float) override;
    virtual const void setMaximumValue (float) override;
        
private:
    
};

#endif
