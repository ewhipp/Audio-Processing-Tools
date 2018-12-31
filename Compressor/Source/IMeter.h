/*
  ==============================================================================

    IMeter.h
    Created: 30 Dec 2018 12:49:09am
    Author:  Erik Whipp

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#ifndef IMETER_H
#define IMETER_H

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
    
    virtual void setCurrentValue             (float) = 0;
    virtual void setValue                    (float) = 0;
    virtual float normalize                   (float) = 0;
    
    virtual void const setType               (int)  noexcept = 0;
};

#endif
