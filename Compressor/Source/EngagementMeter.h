/*
  ==============================================================================

    EngagementMeter.h
    Created: 31 Dec 2018 1:41:36am
    Author:  Erik Whipp

  ==============================================================================
*/

#include "IMeter.h"

#ifndef ENGAGEMENTMETER_H
#define ENGAGEMENTMETER_H

class EngagementMeter :             public IMeter
{
public:
    EngagementMeter (METER_TYPE type = METER_TYPE::ENGAGEMENT, float maximumValue = 1.0f, float minimumValue = 0.0f)
    : m_type (type)
    {
        try
        {
            setType (type);
        }
        catch (MeterInitializationException)
        {
            std::cerr << "Error initializing engagement meter";
        }
        
        setMaximumValue (maximumValue);
        setMinimumValue (minimumValue);
    }
    
    ~EngagementMeter() { }
    
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
