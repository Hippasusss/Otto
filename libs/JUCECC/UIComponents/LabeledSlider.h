#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

class LabeledSlider : public Component
{
public:
    explicit LabeledSlider(const String& name, bool hideLabel = false);
    ~LabeledSlider() = default;

    Slider& getSlider();
    Label& getSliderNameLabel();
    Label& getSliderValueLabel();

    void setName(const String& newName) override;
    void setDefault(float newDefault, bool setValueToo = true);
    void returnToDefault();

private:
    void mouseExit(const MouseEvent&) override;
    void mouseEnter(const MouseEvent&) override;
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;
    void mouseDoubleClick(const MouseEvent&) override;
    void valueChanged();
    void resized() override;

    CustomLookAndFeel2 lookAndFeel2;
    Slider slider;
    Label sliderNameLabel;
    Label sliderValueLabel;
    float defaultValue;
    const String name;
    const bool hiddenLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)
};
