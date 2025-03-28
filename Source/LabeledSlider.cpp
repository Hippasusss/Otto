#include "LabeledSlider.h"

LabeledSlider::LabeledSlider(const String& name, bool hideLabel)
    : sliderNameLabel(name, name),
      defaultValue(0),
      name(name),
      hiddenLabel(hideLabel)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    if(!hideLabel) {
        sliderNameLabel.setName(name);
        sliderNameLabel.setText(name, NotificationType::sendNotification);
        sliderNameLabel.setJustificationType(Justification::centredTop);
        sliderNameLabel.attachToComponent(&slider, true);
        sliderNameLabel.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(sliderNameLabel);
    }

    sliderValueLabel.setJustificationType(Justification::right);
    sliderValueLabel.setInterceptsMouseClicks(false, false);
    sliderValueLabel.setLookAndFeel(&lookAndFeel2);
    sliderValueLabel.setText(String(slider.getValue()), sendNotification);
    sliderValueLabel.setVisible(false);
    sliderValueLabel.attachToComponent(&slider, true);
    addChildComponent(sliderValueLabel);

    slider.onValueChange = [this] { valueChanged(); };
    resized();
}

Label& LabeledSlider::getSliderNameLabel() { return sliderNameLabel; }
Label& LabeledSlider::getSliderValueLabel() { return sliderValueLabel; }
Slider& LabeledSlider::getSlider() { return slider; }

void LabeledSlider::setName(const String& newName) {
    slider.setName(newName);
    sliderNameLabel.setName(newName);
    sliderNameLabel.setText(newName, dontSendNotification);
}

void LabeledSlider::mouseExit(const MouseEvent&) {
    sliderValueLabel.setVisible(false);
}

void LabeledSlider::mouseEnter(const MouseEvent&) {
    sliderValueLabel.setVisible(true);
    if(hiddenLabel) sliderValueLabel.setText(name, NotificationType::sendNotification);
}

void LabeledSlider::mouseDown(const MouseEvent& e) {
    if(hiddenLabel) sliderValueLabel.setText(String(slider.getValue()), sendNotification);
    if(e.mods.isAltDown()) {
        returnToDefault();
    }
}

void LabeledSlider::mouseUp(const MouseEvent&) {
    if(hiddenLabel) sliderValueLabel.setText(name, NotificationType::sendNotification);
}

void LabeledSlider::mouseDoubleClick(const MouseEvent&) {
    returnToDefault();
}

void LabeledSlider::valueChanged() {
    sliderValueLabel.setText(String(slider.getValue()), dontSendNotification);
    resized();
}

void LabeledSlider::resized() {
    slider.setBounds(getLocalBounds().reduced(10));
    const auto bounds = slider.getBounds().reduced(10, slider.getBounds().getHeight()/2 -7);
    sliderValueLabel.setBounds(bounds);
    sliderNameLabel.setBounds(bounds.translated(0, slider.getBounds().getHeight()/2 + sliderNameLabel.getHeight()));
}

void LabeledSlider::setDefault(float newDefault, bool setValueToo) {
    defaultValue = newDefault;
    if(setValueToo) slider.setValue(defaultValue);
}

void LabeledSlider::returnToDefault() {
    slider.setValue(defaultValue);
}

void LabeledSlider::init() {
    resized();
    sliderValueLabel.setVisible(false);
}
