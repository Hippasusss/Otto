#pragma once
#include <JuceHeader.h>

#include "CustomLookAndFeel.h"

class LabeledSlider    : public Slider 
{
public:
	LabeledSlider();
    explicit LabeledSlider(const String& name, const String& ID);
    ~LabeledSlider();

    Label& getSliderNameLabel();
    Label& getSliderValueLabel();
    void setName(const String& newName) override;

	void mouseExit(const MouseEvent&) override;
	void mouseEnter(const MouseEvent&) override;
	void valueChanged() override;
    void resized() override;

private:
    CustomLookAndFeel2 lookAndFeel2;
private:
    Label sliderNameLabel;
    Label sliderValueLabel;
    const String name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)

};
