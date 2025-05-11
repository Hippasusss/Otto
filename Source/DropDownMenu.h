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
#include "juce_gui_basics/juce_gui_basics.h"

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

class DropDownContext : public juce::Component
{
public:
    DropDownContext(Component& parentDDMenu);
    ~DropDownContext() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;
    void addEntry(const String&);
    void addEntries(std::initializer_list<juce::String> entries);

private:
    std::vector<std::unique_ptr<TextButton>> buttonEntries {};
    Component& parent;
};

class DropDownMenu  : public juce::Component
{
public:
    DropDownMenu();
    DropDownMenu(const String&);
    ~DropDownMenu() =  default;

    void paint (juce::Graphics&) override;
    void resized() override;

    void addToDropDownContext(const String& entryToAdd);
    void setCurrentIndex(size_t index);
    DropDownColours colours;

private:

    void init();
    DropDownContext contextMenu;
    String text;
    size_t currentIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropDownMenu)
};

