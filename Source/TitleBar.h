/*
  ==============================================================================

    TitleBar.h
    Created: 13 Jan 2022 1:07:07am
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class TitleBar  : public juce::Component
{
public:
    TitleBar();
    ~TitleBar() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CustomLookAndFeel lookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TitleBar)
};
