/*
  ==============================================================================

    DropDownMenu.h
    Created: 7 May 2025 3:43:27pm
    Author:  danny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Constants.h"

//==============================================================================
/*
*/

struct DropDownColours
{
    juce::Colour backgroundColour = colour_constants::backGround;
    juce::Colour highlightColour = colour_constants::lightMain;
    juce::Colour textColour = colour_constants::main;
    juce::Colour bordorColour = colour_constants::main;
};

class DropDownMenu  : public juce::Component
{
public:
    DropDownMenu();
    ~DropDownMenu() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void addToContents(String entryToAdd);
    void setCurrentIndex(size_t index);
    DropDownColours colours;

private:

    std::vector<String> contents;
    String title;
    size_t currentIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropDownMenu)
};
