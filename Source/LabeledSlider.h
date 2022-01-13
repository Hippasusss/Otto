#pragma once
#include <JuceHeader.h>

#include "CustomLookAndFeel.h"

class LabeledSlider    : public Slider 
{
public:
	LabeledSlider();
    explicit LabeledSlider(const String& name);
    ~LabeledSlider();

    Label& getSliderNameLabel();
    Label& getSliderValueLabel();
    void setName(const String& newName) override;

	void mouseExit(const MouseEvent&) override;
	void mouseEnter(const MouseEvent&) override;
	void mouseDown(const MouseEvent&) override;
	void mouseDoubleClick(const MouseEvent&) override;
	void valueChanged() override;
    void resized() override;
    void setDefault(float newDefault, bool setDefaultToo = true);
    void returnToDefault();
    void init();

private:
    CustomLookAndFeel2 lookAndFeel2;
    Label sliderNameLabel;
    Label sliderValueLabel;
    float defaultValue;
    const String name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)

};
