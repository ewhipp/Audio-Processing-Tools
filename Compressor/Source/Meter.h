/*
  ==============================================================================

    Meter.h
    Created: 31 Dec 2018 10:43:10am
    Author:  Erik Whipp

  ==============================================================================
*/

#ifndef METER_H
#define METER_H

class Meter :           public IMeter
{
public:
    

    Meter (METER_TYPE type): m_type (type)
    {
        try
        {
            setType (m_type);
        }
        catch (MeterInitializationException)
        {
            std::cerr << "ERROR: Meter must be initialized upon creation.\n";
        }
        
        setPaintingIsUnclipped (true);
    }
    ~LevelMeter() { }
    
    void paint (Graphics& /* g */) override;
    
    virtual float normalize (float /* incomingSignal */) override;
    virtual void setValue   (float /* value */) override;
    
    virtual const void setMinimumValue (float /* value */) override;
    virtual const void setMaximumValue (float /* value */) override;
    
    virtual const void setType (int /* type */) noexcept final;
    
    void const chooseDrawing (int /* type */);
    
private:
    METER_TYPE m_type;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};

#endif