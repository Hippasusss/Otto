#include "PluginProcessor.h"
#include "PluginEditor.h"
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
    attack("Atk", true),
    release("Rel", true),
    oversamplingLabel("oversampling:"),
    envAdvancedLabel("env:"),
    oversampling("Over"),
    envSpeed("Slo/Fst"),
    twoFourPole("12/24"),
    envAdvanced(""),
    titleBar("Otto"),
    graphDisplay{
        DisplayData<float>(processor.getGraph()->audioDisplayData, colour_constants::lightMain, true),
        DisplayData<float>(processor.getEnvelopeFollower()->envelopeDisplayData, colour_constants::lightMain, false)
    },
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
    setSize (800, 428);
    
    for(auto& comp : components)
    {
        addAndMakeVisible(comp);
    }

    for (auto& slider : mainSliders)
    {
        mainParameterGroup.addChildComponent(slider);
        slider->setLookAndFeel(&lookAndFeel);
        Label& valueLabel = slider->getSliderValueLabel();
        valueLabel.setColour(Label::ColourIds::backgroundColourId, colour_constants::transparent);
        valueLabel.setColour(Label::ColourIds::textColourId, colour_constants::main);

        Label& nameLabel = slider->getSliderNameLabel();
        nameLabel.setColour(Label::ColourIds::backgroundColourId, colour_constants::main);
        nameLabel.setColour(Label::ColourIds::textColourId, colour_constants::backGround);
    }

    for(auto& slider : timeSliders)
    {
        buttonParameterGroup.addChildComponent(slider);
        slider->setLookAndFeel(&lookAndFeel2);
        auto& valueLabel = slider->getSliderValueLabel();
        valueLabel.setColour(Label::ColourIds::backgroundColourId, colour_constants::transparent);
        valueLabel.setColour(Label::ColourIds::textColourId, colour_constants::main);
        valueLabel.setText(valueLabel.getName(), NotificationType::dontSendNotification);
        valueLabel.setJustificationType(Justification::centred);
    }

    //TODO:: surley not ok thread wise lol
    oversampling.setLookAndFeel(&lookAndFeel2);
    oversampling.addToDropDownContext("Off", [&processor](){processor.changeOversampling(0);});
    oversampling.addToDropDownContext("x2", [&processor](){processor.changeOversampling(1);});
    oversampling.addToDropDownContext("x4", [&processor](){processor.changeOversampling(2);});
    oversampling.addToDropDownContext("x8", [&processor](){processor.changeOversampling(3);});
    oversampling.setText("x2");
    oversamplingLabel.setLookAndFeel(&lookAndFeel2);
    oversamplingLabel.setText("oversampling: ", NotificationType::dontSendNotification);
    oversamplingLabel.setJustificationType(Justification::right);
    buttonParameterGroup.addChildComponent(twoFourPole);
    buttonParameterGroup.addChildComponent(envSpeed);

    envAdvanced.setLookAndFeel(&lookAndFeel2);;
    envAdvanced.setToggleable(true);
    envAdvanced.setColour(ToggleButton::ColourIds::tickColourId, colour_constants::lightMain);
    envAdvanced.setColour(ToggleButton::ColourIds::tickDisabledColourId, colour_constants::backGround);
    envAdvanced.setClickingTogglesState(true);

    envAdvanced.onClick = [this]() {
        resized();
    };
    setLookAndFeel(&lookAndFeel);
    envAdvancedLabel.setLookAndFeel(&lookAndFeel2);
    envAdvancedLabel.setText("Env: ", NotificationType::dontSendNotification);
    envAdvancedLabel.setJustificationType(Justification::right);
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
    for (auto& slider : mainSliders)
    {
        slider->setLookAndFeel(nullptr);
    }
    for(auto& slider : timeSliders)
    {
        slider->setLookAndFeel(nullptr);
    }
    oversampling.setLookAndFeel(nullptr);
    oversamplingLabel.setLookAndFeel(nullptr);
    envAdvancedLabel.setLookAndFeel(nullptr);
    envAdvanced.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

void Auto_AudioProcessorEditor::resized()
{
    // Layout constants
    constexpr int TITLE_BAR_HEIGHT = 28;
    constexpr int ENV_ADVANCED_WIDTH = 30;
    constexpr int ENV_ADVANCED_PADDING = 5;
    constexpr int OVERSAMPLING_WIDTH = 50;
    constexpr int PARAMETER_SECTION_HEIGHT = 100;
    constexpr int SLIDER_PADDING = 12;
    constexpr int SLIDER_EXTRA_TOP_PADDING = 6;
    constexpr int BUTTON_AREA_WIDTH = 100;
    constexpr int BUTTON_PADDING = 10;
    constexpr int BUTTON_HEIGHT_RATIO = 2;
    constexpr int BUTTON_AREA_PAD_TOP_AND_BOTTOM = 6;

    constexpr int SMALL_SLIDER_PADDING = 4;
    constexpr int METER_PADDING_X = 7;
    constexpr int METER_PADDING_Y = 6;
    constexpr int METER_WIDTH = 20;
    constexpr int GRAPH_PADDING = 2;

    auto rect = getLocalBounds();
    // Title Bar --------------------
    auto titleBounds = rect.removeFromTop(TITLE_BAR_HEIGHT);
    titleBar.setBounds(titleBounds);
    envAdvanced.setBounds(titleBounds.removeFromRight(ENV_ADVANCED_WIDTH).reduced(ENV_ADVANCED_PADDING));
    envAdvancedLabel.setBounds(titleBounds.removeFromRight(30));
    oversampling.setBounds(titleBounds.removeFromRight(OVERSAMPLING_WIDTH).reduced(ENV_ADVANCED_PADDING));
    oversamplingLabel.setBounds(titleBounds.removeFromRight(100));

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


