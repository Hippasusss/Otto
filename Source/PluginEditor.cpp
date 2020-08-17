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
    outputMeter(processor.getOutputMeter()),
	graphDisplay(processor.getGraph())

{
    setLookAndFeel(&lookAndFeel);
    setSize (800, 400);

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

    frequency.setSkewFactor(0.5);
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
        addAndMakeVisible(comp);
    }

    // Hide the slider readouts (enabled by mouse over instead)
    for(auto& slider : sliders)
    {
        slider->getSliderValueLabel().setVisible(false);
    }
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    for(auto& slider : sliders)
    {
        slider->removeListener(&processor);
    }

    for(auto& button : buttons)
    {
        button->removeListener(&processor);
    }
}

void Auto_AudioProcessorEditor::resized()
{
    auto rect = getLocalBounds();

    // Parameter Section -------------
    auto parameterBounds = rect.removeFromTop(100);
    parameterGroup.setBounds(parameterBounds);

    // Sliders
    for(auto* slider: sliders)
    {
        parameterGroup.addChildComponent(slider);
        auto bounds = parameterBounds.removeFromLeft(100);
        slider->setBounds(bounds.removeFromTop(80).reduced(11));
        slider->getSliderNameLabel().setBounds(bounds.translated(0,-3));
    }

    // Buttons 
    parameterBounds = parameterBounds.removeFromLeft(100);
    for(auto* button: buttons)
    {
        button->setBounds(parameterBounds.removeFromTop(50).reduced(10,13));
    }

	//----------------------------------

    // Visual Section -------------------

    // Meters
    rect = rect.reduced(6);
    inputMeter.setBounds(rect.removeFromLeft(20));
    outputMeter.setBounds(rect.removeFromRight(20));

	// Waveform Display
    const auto graphRect = rect;
    graphDisplay.setBounds(graphRect);

	//----------------------------------
}

void Auto_AudioProcessorEditor::paint (Graphics& graphics)
{
    graphics.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));
    graphics.drawRect(getBounds(), 4);

}

