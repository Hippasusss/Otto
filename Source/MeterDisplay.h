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
#include "LevelValue.h"
#include "Meter.h"

//==============================================================================
/*
*/
class MeterDisplay    : public Component,
						public Timer
{
public: void timerCallback() override;
	MeterDisplay(Meter* met);
    ~MeterDisplay();

    void paint (Graphics&) override;
	void resized() override;
    void setMeter(Meter* meter);

private:
    Meter* meter;
    ToggleButton clipMeter;
    Label levelText;
    CustomLookAndFeel2 lookAndFeel2;

    int channelCount;
    const float rate = 0.15f;
	const int clipHoldTime = 3000;
	const int peakHoldTime = 1500;
    bool clip;

	int clipHoldTimer = 0;
	int peakHoldTimer = 0;


	LevelValue<float> peak;
	LevelValue<float> peakHold;
	LevelValue<float> RMS;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterDisplay)
};
