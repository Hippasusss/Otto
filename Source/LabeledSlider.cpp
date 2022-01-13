#include <JuceHeader.h>
#include "LabeledSlider.h"



LabeledSlider::LabeledSlider() = default;

LabeledSlider::LabeledSlider(const String& name): sliderNameLabel(name, name) , defaultValue(0)
{
    setSliderStyle (Slider::RotaryVerticalDrag);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    sliderNameLabel.setName(name);
    sliderNameLabel.setText(name, NotificationType::sendNotification);
    sliderNameLabel.setJustificationType(Justification::centredTop);
    sliderNameLabel.attachToComponent(this, true);

    sliderValueLabel.setJustificationType(Justification::right);
    sliderValueLabel.attachToComponent(this, true);
    sliderValueLabel.setInterceptsMouseClicks(false, false);
    sliderValueLabel.setLookAndFeel(&lookAndFeel2);
    sliderValueLabel.setText(String(getValue()), sendNotification);
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

void LabeledSlider::mouseDown(const MouseEvent& mouseEvent)
{
    Slider::mouseDown(mouseEvent);
    if(mouseEvent.mods.isAltDown())
    {
		returnToDefault();
    }
}

void LabeledSlider::mouseDoubleClick(const MouseEvent& mouseEvent)
{
    Slider::mouseDoubleClick(mouseEvent);
    returnToDefault();
}

// TODO: Work out why this resets the child components bounds.
void LabeledSlider::valueChanged()
{
    Slider::valueChanged();
    sliderValueLabel.setText(String(getValue()), dontSendNotification);
    resized();
}

void LabeledSlider::resized()
{
    Slider::resized();
    const auto bounds = getBounds().reduced(10, getBounds().getHeight()/2 -7);
    sliderValueLabel.setBounds(bounds);
}

void LabeledSlider::setDefault(float newDefault, bool setValueToo)
{
    defaultValue = newDefault;
    if(setValueToo) setValue(defaultValue);
}

void LabeledSlider::returnToDefault()
{
    setValue(defaultValue);
}

void LabeledSlider::init()
{
    resized();
    sliderValueLabel.setVisible(false);
}



