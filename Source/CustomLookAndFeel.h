/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 29 Feb 2020 10:09:53pm
    Author:  Danny Herbert

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel: public LookAndFeel_V4 
{
public:
    CustomLookAndFeel()
    {
        setColour(Slider::ColourIds::rotarySliderOutlineColourId, mainColour);
        setColour(ToggleButton::ColourIds::textColourId, mainColour);
        setColour(Label::ColourIds::textColourId, textColour);
        setColour(ResizableWindow::backgroundColourId, backgroundColour);
    }

    void drawToggleButton(Graphics& graphics, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {

        const int width = button.getBounds().getWidth();
        const int height = button.getBounds().getHeight();

        const int fillOffset = button.getToggleState() ? 0 : width / 2;
	    const Rectangle<int> area {0, 0 , width, height};
	    const Rectangle<int> fillArea {fillOffset, 0 , width/2, height};
        graphics.setColour(mainColour);
        graphics.drawRect(area, lineThickness);
        graphics.fillRect(fillArea);
    }

    void drawRotarySlider(Graphics& graphics, int x, int y, int width, int height, 
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        const float value = slider.getValue();
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
	    const int diameter = jmin(width, height) - 5;
        const int radius = diameter / 2;
        const Point<int> offset {(width - diameter) / 2, (height - diameter) / 2};
        const Point<int> centre {width / 2, height / 2};
		const auto dialArea  = Rectangle<float>(x + offset.x, y + offset.y, diameter, diameter);
		const auto textArea  = Rectangle<float>(centre.x - radius, centre.y - radius, diameter, diameter);
		const float pointerLength = radius * 0.2f;

		Path path;
		path.addRectangle (-lineThickness * 0.5f, -radius, lineThickness, pointerLength);
		path.applyTransform (AffineTransform::rotation(angle).translated(centre.x, centre.y));

    	graphics.setColour(textColour);
        //graphics.drawText(String(value), textArea, Justification::centred);
    	graphics.setColour(mainColour);
        graphics.drawEllipse(dialArea, lineThickness);
        graphics.fillPath(path);
    }

	void drawLabel(Graphics& graphics, Label& label) override
    {
        const Font font {"Futara", 15, Font::bold};
        const Justification justification = Justification::centredTop;
	    const Rectangle<int> area = getLocalComponentArea<int>(label);
        const auto text = label.getText();
        graphics.setFont(font);
        graphics.setColour(textColour);
        graphics.drawText(text.toUpperCase(), area, justification);
    }

	void drawGroupComponentOutline(Graphics& graphics, int w, int h, const String& text, const Justification&, GroupComponent& group) override
    {
	    const Rectangle<int> area = getLocalComponentArea<int>(group);
        graphics.setColour(mainColour);
        graphics.drawRect(area, lineThickness);
    }

private:
    const int lineThickness = 4;
    const Colour mainColour {Colours::lightcoral};
	const Colour textColour {Colours::black};
	const Colour backgroundColour {Colours::white};
	
    template<typename T>
    Rectangle<T>&& getLocalComponentArea(const Component& component)
    {
        const int width = component.getBounds().getWidth();
        const int height = component.getBounds().getHeight();
	    return Rectangle<T> {0, 0 , width, height};
	    
    }
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};


