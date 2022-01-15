#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LabeledSlider.h"
#include "Constants.h"

Auto_AudioProcessorEditor::Auto_AudioProcessorEditor (Auto_AudioProcessor& processor)
    : AudioProcessorEditor (&processor), 
    processor (processor), 
    inputMeter(processor.getInputMeter()),
    outputMeter(processor.getOutputMeter()),
    graphDisplay(processor.getGraph()),
    inputGain("In"),
    outputGain("Out"),
    frequency("Freq"),
    resonance("Reso"),
    drive("Drive"),
    mix("Mix"),
    envAmount("Env"),
    inA(processor.apvts, parameter_constants::INPUT_GAIN_ID, inputGain), 
    outA(processor.apvts, parameter_constants::OUTPUT_GAIN_ID, outputGain),
    freqA(processor.apvts, parameter_constants::FREQUENCY_ID, frequency),
    resoA(processor.apvts, parameter_constants::RESONANCE_ID, resonance),
    driveA(processor.apvts, parameter_constants::DRIVE_ID, drive),
    mixA(processor.apvts, parameter_constants::MIX_ID, mix),
    envA(processor.apvts, parameter_constants::ENV_AMOUNT_ID, envAmount),
    twoA(processor.apvts, parameter_constants::TWO_FOUR_POLE_ID, twoFourPole),
    envspA(processor.apvts, parameter_constants::ENV_SPEED_ID, envSpeed)
{
    setLookAndFeel(&lookAndFeel);
    setSize (800, 428);


    // Make all components visible
    for(auto& comp : components)
    {
        addAndMakeVisible(comp);
    }

    for (auto& slide : sliders)
    {
        slide->init();
    }
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void Auto_AudioProcessorEditor::resized()
{
    auto rect = getLocalBounds();
    // Title Bar --------------------
    auto titleBounds = rect.removeFromTop(28);
    titleBar.setBounds(titleBounds);

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
    // TODO:: get the pixels back that are missing at the top of the meter >:|
    rect.reduce(7,6);
    inputMeter.setBounds(rect.removeFromLeft(20));
    outputMeter.setBounds(rect.removeFromRight(20));

	// Waveform Display
    const auto graphRect = rect.reduced(2, 0);
    graphDisplay.setBounds(graphRect);

	//----------------------------------
}

void Auto_AudioProcessorEditor::paint (Graphics& graphics)
{
    graphics.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));
    graphics.drawRect(getBounds(), 4);

}


