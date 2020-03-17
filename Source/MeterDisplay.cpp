/*
  ==============================================================================

    MeterDisplay.cpp
    Created: 8 Mar 2020 6:57:26pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MeterDisplay.h"
#include "Constants.h"

MeterDisplay::MeterDisplay(Meter* newMeter) :
	meter(newMeter),
	channelCount(newMeter->getNumChannels())
{
    meter->onPrepareCallback = [&]()
    {
        channelCount = meter->getNumChannels();
    };
    startTimerHz(timer_constants::REFRESH_RATE);
    addAndMakeVisible(clipMeter);
    addAndMakeVisible(levelText);
}

MeterDisplay::~MeterDisplay()
{
	clipMeter.setLookAndFeel(nullptr);
	levelText.setLookAndFeel(nullptr);
};

void MeterDisplay::paint (Graphics& graphics)
{
    const int separation = 1;
    const int height = getHeight();
    const int width = (getWidth() /  channelCount);

    graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));
    for(int i = 0; i < channelCount; ++i)
    {
        const int leftSeparation = i == 0 ? 0 : separation;
        const int rightSeparation = i == channelCount - 1 ? 0 : separation;

        // Get values of various levels
		RMS = meter->getRMS(i);
		peak = meter->getPeak(i);
        if(peak > peakHold)
        {
			peakHold = peak;
            peakHoldTimer = peakHoldTime;
        }

        //calculate height of each level meter
        const int heightRMS = RMS.getSmoothedValueNormalisedDB() * height;
        const int heightPeak= peak.getSmoothedValueNormalisedDB() * height;
        const int heightPeakHold= peakHold.getSmoothedValueNormalisedDB() * height;
    	
		//draw RMS
        graphics.fillRect((i * width) + leftSeparation , getHeight() - heightRMS, width - leftSeparation - rightSeparation, heightRMS);
		//draw Peak 
        graphics.drawRect((i * width) + leftSeparation , getHeight() - heightPeak, width - leftSeparation- rightSeparation, heightPeak);
        //draw Peak Hold
        graphics.drawRect((i * width) + leftSeparation , getHeight() - heightPeakHold, width - leftSeparation- rightSeparation, 1);
    }

    //set level readout at top of meter
	const float phVal = peakHold.getSmoothedValueDBFS();
    const String labelText = phVal > -60 ? String(phVal, 0): String("");
    levelText.setText(labelText, dontSendNotification);

    //set clip indicator
    clip = meter->getClip();
    if (clip)
    {
        clipMeter.setInterceptsMouseClicks(true, true);
		clipMeter.setToggleState(clip, dontSendNotification);
        clipHoldTimer = clipHoldTime;
    }

    //disallow clicking back on the clip
    else if(!clipMeter.getToggleState())
    {
        clipMeter.setInterceptsMouseClicks(false, false);
    }
        
}

void MeterDisplay::resized() 
{
    clipMeter.setLookAndFeel(&lookAndFeel2);
    clipMeter.setBounds(0,0, getBounds().getWidth(), 5);
    levelText.setLookAndFeel(&lookAndFeel2);
    levelText.setBounds(0,5, getBounds().getWidth(), 15);
}

void MeterDisplay::setMeter(Meter* meter)
{
    this->meter = meter;
}

void MeterDisplay::timerCallback()
{
    if(peakHoldTimer > 0)
    {
    	peakHoldTimer -= 1000 / timer_constants::REFRESH_RATE;
    }
    else
    {
	    peakHold = peak;
    }

    if(clipHoldTime > 0)
    {
    	clipHoldTimer -= 1000 / timer_constants::REFRESH_RATE;
    }
    else
    {
        clipMeter.setToggleState(false, dontSendNotification);
	    clip = false;;
    }
    repaint();
}


