/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 29 Feb 2020 10:09:53pm
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomLookAndFeel    : public LookAndFeel_V4 
{
public:
    CustomLookAndFeel()
    {
        setColour(Slider::ColourIds::textBoxTextColourId, Colours::black);
        setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::black);
        setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colours::black);
        setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
        setColour(ToggleButton::ColourIds::textColourId, Colours::black);
        setColour(ToggleButton::ColourIds::tickColourId, Colours::black);
        setColour(ToggleButton::ColourIds::tickDisabledColourId, Colours::grey);
        setColour(Label::ColourIds::textColourId, Colours::black);
        setColour(ResizableWindow::backgroundColourId, Colours::white);
    }
    void drawRotarySlider(Graphics& graphics, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        const float scale = 0.7;

	    const Point<int> position = slider.getPosition();
        const Point<int> centre {x + width / 2, y + height / 2};

	    const int diameter = (width < height ? width : height) * scale;
        const Point<int> offset {(width - diameter) / 2, (height - diameter) / 2};

		const auto area  = Rectangle<float>(x + offset.x, y + offset.y, diameter, diameter);
    	graphics.setColour(Colours::black);
        graphics.fillEllipse(area);
    	graphics.setColour(Colours::lightblue);
        graphics.drawEllipse(area, 3);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
