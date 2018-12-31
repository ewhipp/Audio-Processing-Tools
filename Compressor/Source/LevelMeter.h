/*
  ==============================================================================

    Meter.h
    Created: 30 Dec 2018 12:57:35am
    Author:  Erik Whipp

    Base class for visual meters.
  ==============================================================================
*/

#include "IMeter.h"
#ifndef METER_H
#define METER_H



/**
 * Base meter class for all types of meters. Future implementations should
 * convert enum types of meter to a specific derived class. Defaults to a
 * simple audio meter that displays level from -100.0 dB to 0.0dB
 *
 * @param m_type: METER_TYPE decides the meter to display. Default : Audio.
 */
class LevelMeter :           public IMeter
{
public:
    

    LevelMeter (METER_TYPE type): m_type (type)
    {
        try
        {
            setType (m_type);
            
            // This is where we need to define the maximum & minimum values for the slider
            if (m_type != METER_TYPE::LEVEL)
            {
                // Error handling for wrong meter ..
                
            }
        }
        catch (MeterInitializationException)
        {
            std::cerr << "ERROR: Meter must be initialized upon creation.\n";
        }
        
        setPaintingIsUnclipped (true);
    }
    ~LevelMeter() { }
    
    void paint (Graphics&) override;
    
    virtual float normalize (float) override;
    virtual void setValue   (float) override;
    
    virtual const void setMinimumValue (float) override;
    virtual const void setMaximumValue (float) override;
    
    virtual const void setType (int) noexcept final;

private:
    METER_TYPE m_type;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};

#endif
