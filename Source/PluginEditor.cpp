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
    attack("Atk"),
    release("Rel"),
    envSpeed("Slo/Fst"),
    twoFourPole("12/24"),
    envAdvanced(""),
    graphDisplay(processor.getGraph()),
    inputMeter(processor.getInputMeter()),
    outputMeter(processor.getOutputMeter()),
    inA(processor.apvts, parameter_constants::INPUT_GAIN_ID, inputGain.getSlider()), 
    driveA(processor.apvts, parameter_constants::DRIVE_ID, drive.getSlider()),
    envA(processor.apvts, parameter_constants::ENV_AMOUNT_ID, envAmount.getSlider()),
    freqA(processor.apvts, parameter_constants::FREQUENCY_ID, frequency.getSlider()),
    resoA(processor.apvts, parameter_constants::RESONANCE_ID, resonance.getSlider()),
    mixA(processor.apvts, parameter_constants::MIX_ID, mix.getSlider()),
    outA(processor.apvts, parameter_constants::OUTPUT_GAIN_ID, outputGain.getSlider()),
    envatA(processor.apvts, parameter_constants::ENV_ATTACK_ID, attack.getSlider()),
    envrelA(processor.apvts, parameter_constants::ENV_RELEASE_ID, release.getSlider()),
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

    for (auto& slider : mainSliders)
    {
        slider->init();
        mainParameterGroup.addChildComponent(slider);
    }

    for(auto& slider : timeSliders)
    {
        buttonParameterGroup.addChildComponent(slider);
    }

    buttonParameterGroup.addChildComponent(twoFourPole);
    buttonParameterGroup.addChildComponent(envSpeed);

    envAdvanced.setToggleable(true);
    envAdvanced.setClickingTogglesState(true);
    envAdvanced.onClick = [this]() {
        resized();
    };
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void Auto_AudioProcessorEditor::resized()
{
    // Layout constants
    constexpr int TITLE_BAR_HEIGHT = 28;
    constexpr int ENV_ADVANCED_WIDTH = 50;
    constexpr int ENV_ADVANCED_PADDING = 5;
    constexpr int PARAMETER_SECTION_HEIGHT = 100;
    constexpr int SLIDER_WIDTH = 100;
    constexpr int SLIDER_HEIGHT = 80;
    constexpr int SLIDER_PADDING = 12;
    constexpr int SLIDER_EXTRA_TOP_PADDING = 6;
    constexpr int BUTTON_AREA_WIDTH = 100;
    constexpr int BUTTON_PADDING = 10;
    constexpr int BUTTON_VERTICAL_PADDING = 5;
    constexpr int BUTTON_HEIGHT_RATIO = 2;
    constexpr int BUTTON_AREA_PAD_TOP_AND_BOTTOM = 4;

    constexpr int SMALL_SLIDER_PADDING = 4;
    constexpr int METER_PADDING_X = 7;
    constexpr int METER_PADDING_Y = 6;
    constexpr int METER_WIDTH = 20;
    constexpr int GRAPH_PADDING = 2;

    auto rect = getLocalBounds();
    // Title Bar --------------------
    auto titleBounds = rect.removeFromTop(TITLE_BAR_HEIGHT);
    envAdvanced.setBounds(titleBounds.removeFromRight(ENV_ADVANCED_WIDTH).reduced(ENV_ADVANCED_PADDING));
    titleBar.setBounds(titleBounds);

    // Parameter Section -------------
    auto parameterBounds = rect.removeFromTop(PARAMETER_SECTION_HEIGHT);
    mainParameterGroup.setBounds(parameterBounds);
    auto localParameterBounds = mainParameterGroup.getLocalBounds();
    localParameterBounds.removeFromTop(SLIDER_EXTRA_TOP_PADDING);
    localParameterBounds.removeFromRight(BUTTON_AREA_WIDTH);
    Helpers::distibuteComponentsEvenlyInRectHorizontally(mainSliders, localParameterBounds, SLIDER_PADDING);

    // Buttons -----------------------
    auto buttonArea = parameterBounds.removeFromRight(BUTTON_AREA_WIDTH);
    buttonParameterGroup.setBounds(buttonArea);
    auto localButtonArea = buttonParameterGroup.getLocalBounds();
    auto topButtonArea = localButtonArea.removeFromTop(buttonArea.getHeight() / BUTTON_HEIGHT_RATIO);
    auto bottomButtonArea = localButtonArea.removeFromTop(buttonArea.getHeight() / BUTTON_HEIGHT_RATIO);
    topButtonArea.removeFromTop(BUTTON_AREA_PAD_TOP_AND_BOTTOM);
    bottomButtonArea.removeFromBottom(BUTTON_AREA_PAD_TOP_AND_BOTTOM);

    if(envAdvanced.getToggleState())
    {
        envSpeed.setVisible(false);
        for(auto& slider : timeSliders) 
        {
            slider->setVisible(true);
        }
        Helpers::distibuteComponentsEvenlyInRectHorizontally(timeSliders, topButtonArea, SMALL_SLIDER_PADDING);
        twoFourPole.setBounds(bottomButtonArea.reduced(BUTTON_PADDING));
    }
    else
    {
        envSpeed.setVisible(true);
        for(auto& slider : timeSliders) 
        {
            slider->setVisible(false);
        }
        envSpeed.setBounds(topButtonArea.reduced(BUTTON_PADDING));
        twoFourPole.setBounds(bottomButtonArea.reduced(BUTTON_PADDING));
    }

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


