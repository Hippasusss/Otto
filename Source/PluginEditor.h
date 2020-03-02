#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LabeledSlider.h"
#include "CustomLookAndFeel.h"

class Auto_AudioProcessorEditor  : public AudioProcessorEditor
{
public:
	Auto_AudioProcessorEditor (Auto_AudioProcessor&);
    ~Auto_AudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Auto_AudioProcessor& processor;
	CustomLookAndFeel lookAndFeel;

    LabeledSlider inputGain;
    LabeledSlider drive;
    LabeledSlider envAmount;
    LabeledSlider frequency;
    LabeledSlider resonance;
    LabeledSlider mix;
    LabeledSlider outputGain;
    ToggleButton envSpeed;
    ToggleButton twoFourPole;
    GroupComponent parameterGroup;

    
    std::array<Component*, 10> components {&inputGain, &drive, &envAmount, &frequency, 
        &resonance, &mix, &outputGain, &envSpeed, &twoFourPole, &parameterGroup};
    std::array<LabeledSlider*, 7> sliders {&inputGain, &drive, &envAmount, &frequency, 
        &resonance, &mix, &outputGain};
    std::array<ToggleButton*, 2> buttons{&envSpeed, &twoFourPole};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Auto_AudioProcessorEditor)
};
