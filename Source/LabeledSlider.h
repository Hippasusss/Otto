#pragma once
#include <JuceHeader.h>

class LabeledSlider    : public Slider 
{
public:
    LabeledSlider();
    explicit LabeledSlider(const String& name, const String& ID);
    ~LabeledSlider();

    Label& getSliderLabel();
    void setName(const String& newName) override;

private:
    Label sliderLabel;
    const juce::String name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)

};
