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
    sliderValueLabel.setText(String(slider.getValue()), sendNotification);
    sliderValueLabel.setVisible(false);
    sliderValueLabel.attachToComponent(&slider, true);
    addChildComponent(sliderValueLabel);
    slider.addMouseListener(this, false);

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
    if(hiddenLabel) sliderValueLabel.setText(name, NotificationType::dontSendNotification);
    sliderValueLabel.setVisible(true);
}

void LabeledSlider::mouseDown(const MouseEvent& e) {
    if(hiddenLabel) sliderValueLabel.setText(String(slider.getValue()), NotificationType::dontSendNotification);
    if(e.mods.isAltDown()) {
        returnToDefault();
    }
}

void LabeledSlider::mouseUp(const MouseEvent&) {
    if(hiddenLabel) sliderValueLabel.setText(name, NotificationType::dontSendNotification);
}

void LabeledSlider::mouseDoubleClick(const MouseEvent&) {
    returnToDefault();
}

void LabeledSlider::valueChanged() {
    sliderValueLabel.setText(String(slider.getValue()), NotificationType::dontSendNotification);
    resized();
}

void LabeledSlider::resized() {
    constexpr float SLIDER_PERCENT_OF_HEIGHT = 0.75;
    constexpr size_t SPACING_BETWEEN_LABEL_AND_SLIDER = 5;
    auto bounds = getLocalBounds();
    const auto valueBounds = getLocalBounds().reduced(10, slider.getBounds().getHeight()/2 -7);
    sliderValueLabel.setBounds(valueBounds);
    if(!hiddenLabel)
    {
        slider.setBounds(bounds.removeFromTop(bounds.getHeight() * SLIDER_PERCENT_OF_HEIGHT));
        bounds.removeFromTop(SPACING_BETWEEN_LABEL_AND_SLIDER);
        sliderNameLabel.setBounds(bounds);
    }
    else
    {
        slider.setBounds(bounds);
        sliderNameLabel.setVisible(false);
    }
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

Label& LabeledSlider::getValueLabel()
{
    return sliderValueLabel;
}

Label& LabeledSlider::getNameLabel()
{
    return sliderNameLabel;
}
