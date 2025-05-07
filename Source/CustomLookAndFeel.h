/*
   ==============================================================================

   CustomLookAndFeel.h
Created: 29 Feb 2020 10:09:53pm
Author:  Danny Herbert

==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "Constants.h"

class CustomLookAndFeel: public LookAndFeel_V4 
{
    public:
const Font font {FontOptions{"Futara", 13, Font::bold}};
CustomLookAndFeel()
{
    setColour(Slider::ColourIds::rotarySliderOutlineColourId, colour_constants::main);
    setColour(ToggleButton::ColourIds::textColourId, colour_constants::main);
    setColour(Label::ColourIds::textColourId, colour_constants::backGround);
    setColour(ResizableWindow::backgroundColourId, colour_constants::backGround);
}

void drawToggleButton(Graphics& graphics, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
{
    const int width = button.getBounds().getWidth();
    const int height = button.getBounds().getHeight();

    int fillOffset = 0;
    String text = button.getButtonText().initialSectionNotContaining("/");
    if(! button.getToggleState())
    {
        text = button.getButtonText().fromFirstOccurrenceOf("/", false, true);
        fillOffset = width / 2;
    }

    const Rectangle<int> area {0, 0 , width, height};
    const Rectangle<int> fillArea {fillOffset, 0 , width/2, height};
    graphics.setColour(colour_constants::main);
    graphics.drawRect(area, lineThickness);
    graphics.fillRect(fillArea);

    graphics.setColour(colour_constants::backGround);
    graphics.setFont(font);
    graphics.drawText(text, fillArea, Justification::centred);
}

void drawRotarySlider(Graphics& graphics, int x, int y, int width, int height, 
                      float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
{
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    const int diameter = jmin(width, height) - 5;
    const int radius = diameter / 2;
    const Point<int> offset {(width - diameter) / 2, (height - diameter) / 2};
    const Point<int> centre {width / 2, height / 2};
    const auto dialArea  = Rectangle<float>(x + offset.x, y + offset.y, diameter, diameter);
    const float pointerLength = radius * 0.2f;

    Path path;
    path.addRectangle (-lineThickness * 0.5f, -radius, lineThickness, pointerLength);
    path.applyTransform (AffineTransform::rotation(angle).translated(centre.x, centre.y));

    graphics.setColour(colour_constants::main);
    graphics.drawEllipse(dialArea, lineThickness);
    graphics.fillPath(path);
}

void drawLabel(Graphics& graphics, Label& label) override
{
    const Justification justification = Justification::centred;
    const Rectangle<int> area = getLocalComponentArea<int>(label);
    const auto text = label.getText();
    auto backgroundColour = label.findColour(Label::ColourIds::backgroundColourId);
    if (backgroundColour == Colour())  // Check if color is unset
        backgroundColour = colour_constants::backGround;

    auto textColour = label.findColour(Label::ColourIds::textColourId);
    if (textColour == Colour())  // Check if color is unset
        textColour = colour_constants::main;
    graphics.setColour(backgroundColour);
    graphics.fillRect(area.reduced(8, 0).removeFromTop(15));
    graphics.setFont(font);
    graphics.setColour(textColour);
    graphics.drawText(text.toUpperCase(), area, justification);
}

void drawGroupComponentOutline(Graphics& graphics, int w, int h, const String& text, const Justification&, GroupComponent& group) override
{
    const Rectangle<int> area = getLocalComponentArea<int>(group);
    graphics.setColour(colour_constants::main);
    graphics.drawRect(area, lineThickness);
}


    private:
const int lineThickness = 3;

template<typename T>
Rectangle<T> getLocalComponentArea(const Component& component)
{
    const int width = component.getBounds().getWidth();
    const int height = component.getBounds().getHeight();
    return Rectangle<T> {0, 0 , width, height};
}

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};


class CustomLookAndFeel2: public CustomLookAndFeel 
{
    public:
CustomLookAndFeel2() {}
const Font font2 {FontOptions{"Futara", 11, Font::bold}};

void drawToggleButton(Graphics& graphics, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
{
    const int width = button.getBounds().getWidth();
    const int height = button.getBounds().getHeight();

    const Rectangle<int> fillArea {0, 0 , width, height};
    graphics.setColour(button.findColour(ToggleButton::ColourIds::tickColourId));

    if(shouldDrawButtonAsHighlighted)
    {
        graphics.drawRect(fillArea);
    }
    if(button.getToggleState())
    {
        graphics.fillRect(fillArea);
    }
    else
{
        graphics.setColour(button.findColour(ToggleButton::ColourIds::tickDisabledColourId));
        graphics.fillRect(fillArea);
    }
}
void drawLabel(Graphics& graphics, Label& label) override
{
    const String text = label.getText();
    const Rectangle<int> fillArea = getLocalComponentArea<int>(label);
    graphics.setFont(font2);
    graphics.setColour(colour_constants::main);
    graphics.drawText(text, fillArea, Justification::centred, false);
}

void drawGroupComponentOutline(Graphics& graphics, int w, int h, const String& text, const Justification&, GroupComponent& group) override
{
    const Rectangle<int> area = getLocalComponentArea<int>(group);
    graphics.setColour(colour_constants::main);
    graphics.fillRect(area);
}


    private:
const int lineThickness = 4;

template<typename T>
Rectangle<T> getLocalComponentArea(const Component& component)
{
    const int width = component.getBounds().getWidth();
    const int height = component.getBounds().getHeight();
    return Rectangle<T> {0, 0 , width, height};
}

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel2)
};


