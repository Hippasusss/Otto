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
    envSpeed("Slo/Fst"),
    twoFourPole("12/24"),
    envAdvanced(""),
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

    envAdvanced.setToggleable(true);
    envAdvanced.setClickingTogglesState(true);
    envAdvanced.onClick = [this]() {
        resized(); 
        repaint();
    };
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void Auto_AudioProcessorEditor::resized()
{
    // Layout constants
    const int TITLE_BAR_HEIGHT = 28;
    const int ENV_ADVANCED_WIDTH = 50;
    const int ENV_ADVANCED_PADDING = 5;
    const int PARAMETER_SECTION_HEIGHT = 100;
    const int SLIDER_WIDTH = 100;
    const int SLIDER_HEIGHT = 80;
    const int SLIDER_PADDING = 11;
    const int LABEL_Y_OFFSET = -3;
    const int BUTTON_PADDING = 10;
    const int BUTTON_VERTICAL_PADDING = 5;
    const int BUTTON_HEIGHT_RATIO = 2; // 0.5 of height
    const int METER_PADDING_X = 7;
    const int METER_PADDING_Y = 6;
    const int METER_WIDTH = 20;
    const int GRAPH_PADDING = 2;

    auto rect = getLocalBounds();
    
    // Title Bar --------------------
    auto titleBounds = rect.removeFromTop(TITLE_BAR_HEIGHT);
    envAdvanced.setBounds(titleBounds.removeFromRight(ENV_ADVANCED_WIDTH).reduced(ENV_ADVANCED_PADDING));
    titleBar.setBounds(titleBounds);

    // Parameter Section -------------
    auto parameterBounds = rect.removeFromTop(PARAMETER_SECTION_HEIGHT);
    parameterGroup.setBounds(parameterBounds);

    // Sliders
    for(auto* slider: mainSliders)
    {
        parameterGroup.addChildComponent(slider);
        auto bounds = parameterBounds.removeFromLeft(SLIDER_WIDTH);
        slider->setBounds(bounds.removeFromTop(SLIDER_HEIGHT).reduced(SLIDER_PADDING));
        slider->getSliderNameLabel().setBounds(bounds.translated(0, LABEL_Y_OFFSET));
    }

    // Buttons 
    parameterBounds = parameterBounds.removeFromLeft(SLIDER_WIDTH);
    parameterBounds.reduce(0, BUTTON_VERTICAL_PADDING);
    auto topButtonArea = parameterBounds.removeFromTop(parameterBounds.getHeight() / BUTTON_HEIGHT_RATIO);
    
    if(envAdvanced.getToggleState())
    {
        envSpeed.setVisible(false);
        for(auto& slider : timeSliders) 
        {
            slider->setBounds(topButtonArea.removeFromLeft(parameterBounds.getWidth() / BUTTON_HEIGHT_RATIO).reduced(0, BUTTON_VERTICAL_PADDING));
            slider->setVisible(true);
        }
    }
    else
    {
        for(auto& slider : timeSliders) 
        {
            slider->setVisible(false);
        }
        envSpeed.setVisible(true);
        envSpeed.setBounds(topButtonArea.reduced(BUTTON_PADDING, BUTTON_PADDING * 0.6f));
    }
    twoFourPole.setBounds(parameterBounds.reduced(BUTTON_PADDING, BUTTON_PADDING * 0.6f));

    // Visual Section -------------------
    // Meters
    rect.reduce(METER_PADDING_X, METER_PADDING_Y);
    inputMeter.setBounds(rect.removeFromLeft(METER_WIDTH));
    outputMeter.setBounds(rect.removeFromRight(METER_WIDTH));

    // Waveform Display
    const auto graphRect = rect.reduced(GRAPH_PADDING, 0);
    graphDisplay.setBounds(graphRect);
}

void Auto_AudioProcessorEditor::paint (Graphics& graphics)
{
    graphics.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));
    graphics.drawRect(getBounds(), 4);

}


