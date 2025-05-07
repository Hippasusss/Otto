/*
  ==============================================================================

    DropDownMenu.h
    Created: 7 May 2025 3:43:27pm
    Author:  danny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DropDownMenu  : public juce::Component
{
public:
    DropDownMenu();
    ~DropDownMenu() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropDownMenu)
};
