#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LabeledSlider.h"
#include "Constants.h"

Auto_AudioProcessorEditor::Auto_AudioProcessorEditor (Auto_AudioProcessor& processor)
    : AudioProcessorEditor (&processor), 
    processor (processor), 
    inputGain("In", parameter_constants::INPUT_GAIN_ID), 
    drive("Drive", parameter_constants::DRIVE_ID),
    envAmount("Env Am", parameter_constants::ENV_AMOUNT_ID),
    frequency("Freq", parameter_constants::FREQUENCY_ID),
    resonance("Res", parameter_constants::RESONANCE_ID),
    mix("Mix", parameter_constants::MIX_ID),
    outputGain("Out", parameter_constants::OUTPUT_GAIN_ID),
    envSpeed("Fast/Slow", parameter_constants::ENV_SPEED_ID),
    twoFourPole("2/4", parameter_constants::TWO_FOUR_POLE_ID),
    parameterGroup("parameterGroup"),
	inputMeter(processor.getInputMeter()),
	outputMeter(processor.getOutputMeter())
{
    setLookAndFeel(&lookAndFeel);
    setSize (800, 300);

    // Default setup for sliders
    for(auto& slider : sliders)
    {
        slider->setSliderStyle (Slider::RotaryVerticalDrag);
        slider->setRange(0, 1.0, 0.01);
        slider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true,0,0);
        slider->setDefault(0.0);
        slider->addListener(&processor);
    }

    // Specific setup for sliders
    inputGain. setRange(-30, 30,    0.1);
    outputGain.setRange(-30, 30,    0.1);
    drive.     setRange(1,   10,    0.1);
    resonance. setRange(0.0, 1.0,   0.001);
    frequency. setRange(20,  20000, 0.001);
    mix.       setRange(0,   100,   0.001);
    envAmount. setRange(0,   100,   0.1);

    frequency.setSkewFactorFromMidPoint(500);
	frequency.setDefault(1000.0f);

	mix.setDefault(100.0f);

    // Default setup for buttons
    for(auto& button : buttons)
    {
        button->setButtonText(button->getName());
        button->addListener(&processor);
    }

    // Make all components visible
    for(auto& comp : components)
    {
        if(comp != &parameterGroup)
        {
            parameterGroup.addChildComponent(comp);
        }
        addAndMakeVisible(comp);
    }

	// Hide the slider readouts (enabled by mouse over)
    for(auto& slider : sliders)
    {
	    slider->getSliderValueLabel().setVisible(false);
    }

    // Callback to update the filter follower graphic
	display.onAddValue = [&]() -> float
    {
    	return processor.getEnvelopeFollower().getValue();
    };

    // Callback to update the filter follower graphic offset
	display.onSetValueOffset = [&]() -> float
    {
		auto range = NormalisableRange<float>(frequency.getMinimum(), frequency.getMaximum(), frequency.getValue());
    	return range.convertTo0to1(frequency.getValue());
    };

    // Ref to update the i/o meters display
	inputMeter.setMeter(processor.getInputMeter());
	outputMeter.setMeter(processor.getOutputMeter());

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

    // Top section with Graph and Meters
    auto topRect = rect.removeFromTop(200).reduced(5,1);
    topRect.removeFromTop(4);
    inputMeter.setBounds(topRect.removeFromLeft(20));
    outputMeter.setBounds(topRect.removeFromRight(20));
    display.setBounds(topRect);


    // Bottom Section
    parameterGroup.setBounds(rect);
    auto tempRect = rect;

    // Sliders
    for(auto slider: sliders)
    {
        tempRect = rect.removeFromLeft(100);
        slider->setBounds(tempRect.removeFromTop(80).reduced(11));
        slider->getSliderNameLabel().setBounds(tempRect.translated(0,-3));
    }

    // Buttons 
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

