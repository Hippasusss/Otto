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
class DropDownMenu;

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

//==================================================================================

class DropDownContext : public juce::Component
{
public:
    DropDownContext(DropDownMenu& parentDDMenu);
    ~DropDownContext() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;
    void addEntry(const String&, std::function<void()>);

private:
    std::vector<std::unique_ptr<TextButton>> buttonEntries {};
    DropDownMenu& parent;
};

//===================================================================================

class DropDownMenu  : public juce::Component
{
public:
    DropDownMenu();
    DropDownMenu(const String&);
    ~DropDownMenu() =  default;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setText(const String&);
    void addToDropDownContext(const String& entryToAdd, std::function<void()>);

private:

    void lookAndFeelChanged() override;
    void mouseUp(const juce::MouseEvent& event) override;
    DropDownColours colours;
    void init();
    DropDownContext contextMenu;
    String text;
    size_t currentIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropDownMenu)
};

