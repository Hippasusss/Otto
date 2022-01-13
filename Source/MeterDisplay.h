/*
   ==============================================================================

MeterDisplay.h
Created: 8 Mar 2020 6:57:26pm
Author:  Danny Herbert

==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "LevelValueDisplay.h"
#include "Meter.h"

//==============================================================================
/*
*/
class MeterDisplay    : public Component,
    public Timer
{
    public: void timerCallback() override;
        MeterDisplay(Meter* newMeter);
        ~MeterDisplay();

        void paint (Graphics&) override;
        void resized() override;
        void setMeter(Meter* newMeter);

    private:
        Meter* meter;
        ToggleButton clipMeter;
        Label levelText;
        CustomLookAndFeel2 lookAndFeel2;

        const float rate = 0.15f;
        const int clipHoldTime = 3000;
        const int peakHoldTime = 1500;

        int channelCount = 0;
        bool clip = false;
        bool vertical = false;
        int clipHoldTimer = 0;
        int peakHoldTimer = 0;

        LevelValueDisplay<float> peak;
        LevelValueDisplay<float> peakHold;
        LevelValueDisplay<float> RMS;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterDisplay)
};
