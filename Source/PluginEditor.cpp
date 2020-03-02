#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LabeledSlider.h"
#include "parameter_constants.h"

Auto_AudioProcessorEditor::Auto_AudioProcessorEditor (Auto_AudioProcessor& p)
    : AudioProcessorEditor (&p), 
    processor (p), 
    inputGain("In", parameter_constants::INPUT_GAIN_ID), 
    drive("Drive", parameter_constants::DRIVE_ID),
    envAmount("Env Am", parameter_constants::ENV_AMOUNT_ID),
    frequency("Freq", parameter_constants::FREQUENCY_ID),
    resonance("Res", parameter_constants::RESONANCE_ID),
    mix("Mix", parameter_constants::MIX_ID),
    outputGain("Out", parameter_constants::OUTPUT_GAIN_ID),
    envSpeed("Env Speed", parameter_constants::ENV_SPEED_ID),
    twoFourPole("2/4 Pole", parameter_constants::TWO_FOUR_POLE_ID),
    parameterGroup("parameterGroup")
{
    setLookAndFeel(&lookAndFeel);
    setSize (800, 400);
    for(auto slider : sliders)
    {
        slider->setSliderStyle (Slider::RotaryVerticalDrag);
        slider->setRange(0, 1.0, 0.01);
        slider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true,0,0);
        slider->setValue(0.0);
        slider->addListener(&processor);
    }

    inputGain.setRange(Range<double>(-30, 30), 0.1);
    outputGain.setRange(Range<double>(-30, 30), 0.1);
    drive.setRange(Range<double>(1, 10), 0.1);
    resonance.setRange(Range<double>(0.0, 1.0), 0.001);

    frequency.setRange(Range<double>(20, 20000), 0.1);
    frequency.setSkewFactorFromMidPoint(2000);
	frequency.setValue(1000.0f);

	mix.setValue(1.0f);


    for(auto button : buttons)
    {
        button->setButtonText(button->getName());
        button->addListener(&processor);
    }

    for(auto comp : components)
    {
        if(comp != &parameterGroup)
        {
            parameterGroup.addChildComponent(comp);
        }
        addAndMakeVisible(comp);
    }
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
	
    setLookAndFeel(nullptr);
    for(auto slider : sliders)
    {
        slider->removeListener(&processor);
    }

    for(auto button : buttons)
    {
        button->removeListener(&processor);
    }
}

void Auto_AudioProcessorEditor::resized()
{
    auto rect = getLocalBounds();
    auto topRect = rect.removeFromTop(300);

    parameterGroup.setBounds(rect);
    auto tempRect = rect;
    for(auto slider: sliders)
    {
        tempRect = rect.removeFromLeft(100);
        slider->setBounds(tempRect.removeFromTop(80).reduced(11));
        slider->getSliderLabel().setBounds(tempRect.translated(0,-3));
    }

    tempRect = rect.removeFromLeft(100);
    for(auto button: buttons)
    {
        button->setBounds(tempRect.removeFromTop(50).reduced(10,13));
    }
}

void Auto_AudioProcessorEditor::paint (Graphics& graphics)
{
    graphics.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));
    graphics.drawRect(getBounds(), 4);

}

