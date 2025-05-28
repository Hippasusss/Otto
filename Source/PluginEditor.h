#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LabeledSlider.h"
#include "CustomLookAndFeel.h"
#include "GraphDisplay.h"
#include "MeterDisplay.h"
#include "TitleBar.h"
#include "DropDownMenu.h"
// #include "melatonin_inspector/melatonin_inspector.h"

class Auto_AudioProcessorEditor  : public AudioProcessorEditor
{
public:
	Auto_AudioProcessorEditor (Auto_AudioProcessor&);
    ~Auto_AudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Auto_AudioProcessor& processor;

    LabeledSlider inputGain, drive, envAmount, frequency, resonance, mix, outputGain;
    LabeledSlider attack, release;
    Label oversamplingLabel;
    DropDownMenu oversampling;
    ToggleButton envSpeed, twoFourPole, envAdvanced;
    GroupComponent mainParameterGroup, buttonParameterGroup;
    TitleBar titleBar;
    GraphDisplay graphDisplay;
    MeterDisplay inputMeter, outputMeter;
    CustomLookAndFeel lookAndFeel;
    CustomLookAndFeel2 lookAndFeel2;
    // melatonin::Inspector inspector { *this };
    
    std::array<Component*, 20> components 
       {&inputGain, 
        &drive, 
        &envAmount, 
        &frequency, 
        &resonance, 
        &mix, 
        &outputGain, 
        &attack, 
        &release, 
        &envSpeed, 
        &twoFourPole, 
        &mainParameterGroup, 
        &buttonParameterGroup, 
        &graphDisplay,
        &inputMeter, 
        &outputMeter, 
        &titleBar, 
        &oversamplingLabel,
        &oversampling,
        &envAdvanced};
    std::array<LabeledSlider*, 7> mainSliders {&inputGain, &drive, &envAmount, &frequency, &resonance, &mix, &outputGain};
    std::array<LabeledSlider*, 2> timeSliders{&attack, &release};
    std::array<ToggleButton*, 3> buttons{&envSpeed, &twoFourPole, &envAdvanced};

    AudioProcessorValueTreeState::SliderAttachment inA, driveA, envA, freqA, resoA, mixA, outA, envatA, envrelA;
    AudioProcessorValueTreeState::ButtonAttachment envspA, twoA, envadvA;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Auto_AudioProcessorEditor)
};
