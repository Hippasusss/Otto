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
    slider.addMouseListener(this, true);
    slider.onValueChange = [this] { valueChanged(); };

    if(!hiddenLabel) 
    {
        sliderNameLabel.setName(name);
        sliderNameLabel.setText(name, NotificationType::sendNotification);
        sliderNameLabel.setInterceptsMouseClicks(false, false);
        sliderNameLabel.setVisible(true);
        sliderNameLabel.setJustificationType(Justification::centred);
        addChildComponent(sliderNameLabel);
    }

    auto value = slider.getValue();
    auto stringValue = static_cast<String>(value);

    if(hiddenLabel) 
    {
        sliderValueLabel.setText(name, NotificationType::dontSendNotification);
        sliderValueLabel.setVisible(true);
    }
    else 
    {
        sliderValueLabel.setText(String(slider.getValue()), NotificationType::dontSendNotification);
        sliderValueLabel.setVisible(false);
    }
    sliderValueLabel.setJustificationType(Justification::centred);
    sliderValueLabel.setName(name);
    sliderValueLabel.setInterceptsMouseClicks(false, false);
    addChildComponent(sliderValueLabel);

    resized();
}

Label& LabeledSlider::getSliderNameLabel() { return sliderNameLabel; }
Label& LabeledSlider::getSliderValueLabel() { return sliderValueLabel; }
Slider& LabeledSlider::getSlider() { return slider; }

void LabeledSlider::setName(const String& newName) 
{
    slider.setName(newName);
    sliderNameLabel.setName(newName);
    sliderValueLabel.setName(newName);
    sliderNameLabel.setText(newName, dontSendNotification);
}

void LabeledSlider::mouseExit(const MouseEvent&) 
{
    if (!hiddenLabel) sliderValueLabel.setVisible(false);
    resized();
}

void LabeledSlider::mouseEnter(const MouseEvent&) 
{
    if(hiddenLabel) { sliderValueLabel.setText(name, NotificationType::dontSendNotification); }
    else { sliderValueLabel.setText(String(slider.getValue()), NotificationType::dontSendNotification); }
    sliderValueLabel.setVisible(true);
    resized();
}

void LabeledSlider::mouseDown(const MouseEvent& e) 
{
    if(hiddenLabel) sliderValueLabel.setText(String(slider.getValue()), NotificationType::dontSendNotification);
    if(e.mods.isAltDown()) returnToDefault();
    resized();
}

void LabeledSlider::mouseUp(const MouseEvent&) 
{
    if(hiddenLabel) sliderValueLabel.setText(name, NotificationType::dontSendNotification);
}

void LabeledSlider::mouseDoubleClick(const MouseEvent&) 
{
    returnToDefault();
}

void LabeledSlider::valueChanged() 
{
    sliderValueLabel.setText(String(slider.getValue()), NotificationType::dontSendNotification);
    resized();
}

void LabeledSlider::resized() 
{
    constexpr float SLIDER_PERCENT_OF_HEIGHT = 0.75;
    constexpr size_t SPACING_BETWEEN_LABEL_AND_SLIDER = 5;
    Rectangle<int> bounds = getLocalBounds();

    Rectangle<int> sliderBounds;
    Rectangle<int> NameLabelBounds;
     

    if(!hiddenLabel)
    {
        sliderBounds = bounds.removeFromTop(bounds.getHeight() * SLIDER_PERCENT_OF_HEIGHT);
        bounds.removeFromTop(SPACING_BETWEEN_LABEL_AND_SLIDER);
        NameLabelBounds = bounds;
    }
    else
    {
        sliderBounds = bounds;
    }
    slider.setBounds(sliderBounds);
    sliderNameLabel.setBounds(NameLabelBounds);
    sliderValueLabel.setBounds(sliderBounds);
}

void LabeledSlider::setDefault(float newDefault, bool setValueToo) 
{
    defaultValue = newDefault;
    if(setValueToo) slider.setValue(defaultValue);
}

void LabeledSlider::returnToDefault() 
{
    slider.setValue(defaultValue);
}

