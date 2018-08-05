/*
  ==============================================================================

    EngagementMeter.h
    Created: 5 Aug 2018 2:18:19pm
    Author:  Erik Whipp

    This class is a visual cue for the user to see how much the processor is
    engaged. As an example, the GUI will present itself as a half circle with
    a line that moves based on the gain factor? of the compressor.
  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/**
 * Preplanning:
 *  - Simple line for the base of the component.
 *  - A triangle for the actual meter of the component.
 *  - The triangle will move as a function of a sin wave mapped across the diameter
 *      of the base of the component.
 *      - The amount at which it moves is based on the current gainFactor in the
 *          processor class.
 */
 
class EngagementMeter : public Component
{
public:
    EngagementMeter();
    ~EngagementMeter();
    void paint(Graphics&) override;
    
    
private:
    
};
