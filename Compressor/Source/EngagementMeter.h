/*
  ==============================================================================

    EngagementMeter.h
    Created: 5 Aug 2018 2:18:19pm
    Author:  Erik Whipp

    This class is a visual cue for the user to see how much the processor is
    engaged. As an example, the GUI will present itself as a half circle with
    a line that moves based on the gain factor? of the compressor.
 
    The class begins with some pre-defined rectangle properties and a line from
    the middle of said rectangle to the left. The line must move in accordance
    with whatever incoming signal is given to the `moveMeter` function.
  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioMeter.h"

/**
 * Preplanning:
 *  - A Line for the actual meter of the component.
 *  - The triangle will move as a function of a sin wave mapped across the diameter
 *      of the base of the component.
 *      - The amount at which it moves is based on the current gainFactor in the
 *          processor class.
 */
 
class EngagementMeter : public Component
{
public:
    
    EngagementMeter (int);
    ~EngagementMeter();
    void paint(Graphics&) override;
    
    /**
     * Define the intensity of the engagement based on the minimum and maximum
     * values allowed for the path to move.
     *
     * @param: Incoming signal to be measured.
     */
    void setVisualMeterLevel (float);

    
    /**
     * For more utility, we want to have the ability to normalize the signal based
     * on the width of the rectangle.
     *
     * @param: Incoming signal to be normalized.
     */
    float normalize (float);
    
    void setType (int);
    void setMinimumValue (float);
    void setMaximumValue (float);
    void setCurrentLevel (float);
    void setMeterLimits();
    
private:
    
    /**
     * This is the structure for our engagement meter. It is a basic black rectange
     * with an arrow that will move based on the incoming signal.
     */
    struct VisualMeter : public Component
    {
        VisualMeter() { setPaintingIsUnclipped (true); }
        
        const int meterMidpoint         = 150;
        const int meterBottomMidpoint   = 200;
        const int bottomMeasurePoint    = 0;
        const int leftMostPoint         = 200;
        
        const float arrowThickness = 2.314;
        const float arrowLength    = 16.361;
        const float arrowTriangle  = 13.064;
         
        void paint (Graphics& g) override
        {
            Line<float> arrowLine (Point<float> (meterMidpoint, meterBottomMidpoint),
                                   Point<float> (bottomMeasurePoint, leftMostPoint));
            
            g.setColour (Colours::black);
            g.fillRect (getLocalBounds());
            
            g.setColour (Colours::red);
            g.drawArrow (arrowLine, arrowThickness, arrowLength, arrowTriangle);
        }
        
        
    };
    
    /**
     * TODO: Create different sizes based on the current dimensions of the screen
     */
    enum
    {
        small = 0,
        medium,
        large,
        extraLarge
    };
    
    float level, minimumValue, maximumValue;
    VisualMeter v_Meter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EngagementMeter)
};
