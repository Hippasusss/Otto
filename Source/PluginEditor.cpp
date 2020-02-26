#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LabeledSlider.h"

Auto_AudioProcessorEditor::Auto_AudioProcessorEditor (Auto_AudioProcessor& p)
    : AudioProcessorEditor (&p), 
    processor (p), 
    inputGain("In", "inputGain"), 
    drive("Drive", "drive"),
    envAmount("Env Am", "envAmount"),
    frequency("Freq", "frequency"),
    resonance("Res", "resonance"),
    mix("Mix", "mix"),
    outputGain("Out", "outputGain"),
    envSpeed("Env Speed", "envSpeed"),
    twoFourPole("2/4 Pole", "twoFourPole")
{
    setSize (800, 100);

    for(auto slider : sliders)
    {
        slider->setSliderStyle (Slider::RotaryVerticalDrag);
        slider->setRange(0, 1.0, 0.01);
        slider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        slider->setValue(0.0);
        slider->addListener(&processor);
    }

    for(auto button : buttons)
    {
        button->setButtonText(button->getName());
        button->addListener(&processor);
    }

    for(auto comp : components)
    {
        addAndMakeVisible(comp);
    }
}

Auto_AudioProcessorEditor::~Auto_AudioProcessorEditor()
{
	
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

    auto tempRect = rect;
    for(auto slider: sliders)
    {
        tempRect = rect.removeFromLeft(100);
        slider->setBounds(tempRect.removeFromTop(80));
        slider->getSliderLabel().setBounds(tempRect);
    }

    tempRect = rect.removeFromLeft(100);
    for(auto button: buttons)
    {
        button->setBounds(tempRect.removeFromTop(50));
    }
}

void Auto_AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

