#include <JuceHeader.h>
#include "LabeledSlider.h"

LabeledSlider::LabeledSlider() = default;

LabeledSlider::LabeledSlider(const String& name, const String& ID): sliderLabel(name, name) 
{
    setComponentID(ID);
    sliderLabel.setJustificationType(Justification::centred);
    sliderLabel.attachToComponent(this, true);
    addAndMakeVisible(sliderLabel);
}

LabeledSlider::~LabeledSlider() = default;

Label& LabeledSlider::getSliderLabel()
{
    return sliderLabel;
}

void LabeledSlider::setName(const String& newName)
{
    Slider::setName(newName);
    sliderLabel.setName(newName);
    sliderLabel.setText(newName, dontSendNotification);
}


