#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LabeledSlider.h"
#include "Constants.h"

Auto_AudioProcessorEditor::Auto_AudioProcessorEditor (Auto_AudioProcessor& processor)
    : AudioProcessorEditor (&processor), 
    processor (processor), 
    inputGain("In"),
    drive("Drive"),
    envAmount("Env"),
    frequency("Freq"),
    resonance("Reso"),
    mix("Mix"),
    outputGain("Out"),
    attack("Attack"),
    release("Release"),
    envSpeed("Speed"),
    twoFourPole("Mode"),
    envAdvanced("Advanced"),
    graphDisplay(processor.getGraph()),
    inputMeter(processor.getInputMeter()),
    outputMeter(processor.getOutputMeter()),
    inA(processor.apvts, parameter_constants::INPUT_GAIN_ID, inputGain), 
    driveA(processor.apvts, parameter_constants::DRIVE_ID, drive),
    envA(processor.apvts, parameter_constants::ENV_AMOUNT_ID, envAmount),
    freqA(processor.apvts, parameter_constants::FREQUENCY_ID, frequency),
    resoA(processor.apvts, parameter_constants::RESONANCE_ID, resonance),
    mixA(processor.apvts, parameter_constants::MIX_ID, mix),
    outA(processor.apvts, parameter_constants::OUTPUT_GAIN_ID, outputGain),
    envatA(processor.apvts, parameter_constants::ENV_ATTACK_ID, attack),
    envrelA(processor.apvts, parameter_constants::ENV_RELEASE_ID, release),
    envspA(processor.apvts, parameter_constants::ENV_SPEED_ID, envSpeed),
    twoA(processor.apvts, parameter_constants::TWO_FOUR_POLE_ID, twoFourPole),
    envadvA(processor.apvts, parameter_constants::ENV_ADVANCED_ID, envAdvanced)
{
    setLookAndFeel(&lookAndFeel);
    setSize (800, 428);


    // Make all components visible
    for(auto& comp : components)
    {
        addAndMakeVisible(comp);
    }

    for (auto& slide : mainSliders)
    {
        slide->init();
    }

    for (auto& slide : timeSliders)
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
    //TODO: replace magic numbers with consts or somethign more flexy
    auto rect = getLocalBounds();
    // Title Bar --------------------
    auto titleBounds = rect.removeFromTop(28);
    titleBar.setBounds(titleBounds);

    // Parameter Section -------------
    auto parameterBounds = rect.removeFromTop(100);
    parameterGroup.setBounds(parameterBounds);

    // Sliders
    for(auto* slider: mainSliders)
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
        if(button->getComponentID() == envSpeed.getComponentID())
        {
            if(envAdvanced.getToggleState())
            {
                button->setBounds(parameterBounds.removeFromTop(50).reduced(10,13));
            }
            else
            {
                auto ARrect = parameterBounds.removeFromTop(50).reduced(5,5);
                for(auto* slider : timeSliders)
                {
                    slider->setBounds(ARrect.removeFromLeft(50).reduced(5,5));
                }
            }
        }
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


