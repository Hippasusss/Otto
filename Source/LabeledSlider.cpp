#include <JuceHeader.h>
#include "LabeledSlider.h"


LabeledSlider::LabeledSlider() = default;

LabeledSlider::LabeledSlider(const String& name, const String& ID): sliderNameLabel(name, name) 
{
    setComponentID(ID);
    sliderNameLabel.setJustificationType(Justification::centredTop);
    sliderNameLabel.attachToComponent(this, true);
    addAndMakeVisible(sliderNameLabel);

    sliderValueLabel.setJustificationType(Justification::centred);
    sliderValueLabel.attachToComponent(this, true);
    sliderValueLabel.setInterceptsMouseClicks(false,false);
    sliderValueLabel.setLookAndFeel(&lookAndFeel2);
    sliderValueLabel.setText(String(getValue()), dontSendNotification);
    addAndMakeVisible(sliderValueLabel);
    sliderValueLabel.setVisible(false);
}

LabeledSlider::~LabeledSlider() = default;

Label& LabeledSlider::getSliderNameLabel()
{
    return sliderNameLabel;
}

Label& LabeledSlider::getSliderValueLabel()
{
    return sliderValueLabel;
}

void LabeledSlider::setName(const String& newName)
{
    Slider::setName(newName);
    sliderNameLabel.setName(newName);
    sliderNameLabel.setText(newName, dontSendNotification);
}

void LabeledSlider::mouseExit(const MouseEvent& mouseEvent)
{
    sliderValueLabel.setVisible(false);
}

void LabeledSlider::mouseEnter(const MouseEvent& mouseEvent)
{
    sliderValueLabel.setVisible(true);
}

//TODO: work out why this resets the child components bounds
void LabeledSlider::valueChanged()
{
    Slider::valueChanged();
    sliderValueLabel.setText(String(getValue()), dontSendNotification);
    auto bounds = getBounds().reduced(10, getBounds().getHeight()/2 -10);
    sliderValueLabel.setBounds(bounds);
}

void LabeledSlider::resized()
{
    Slider::resized();
    auto bounds = getBounds().reduced(10, getBounds().getHeight()/2 -10);
    sliderValueLabel.setBounds(bounds);
    sliderValueLabel.setVisible(false);
}



