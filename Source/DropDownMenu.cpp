/*
  ==============================================================================

    DropDownMenu.cpp
    Created: 7 May 2025 3:43:27pm
    Author:  danny

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DropDownMenu.h"

//==============================================================================
DropDownMenu::DropDownMenu()
{
}

DropDownMenu::~DropDownMenu()
{
}

void DropDownMenu::paint (juce::Graphics& g)
{

    const auto& localBounds = getLocalBounds();
    g.setColour(colour_constants::main);
    g.fillRect(localBounds);

    g.setColour (colour_constants::backGround);
    g.drawRect (localBounds, 1);

    g.setColour (colour_constants::backGround);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText (title, localBounds, juce::Justification::centred, true);  
    for (size_t i = 0; i < contents.size(); ++i) 
    {
        const int initialY = (i + 2) * localBounds.getHeight();
        const int initialX = 0;
        const int width = localBounds.getWidth();
        const int height = localBounds.getHeight();
        Rectangle<int> bounds = {initialX, initialY, width, height };
        g.setColour(colour_constants::main);
        g.fillRect(bounds);

        g.setColour (juce::Colours::white);
        g.setFont (juce::FontOptions (14.0f));
        g.drawText (title, getLocalBounds(), juce::Justification::centred, true);  
    };

    Rectangle<int> bounds = {0, localBounds.getHeight(), localBounds.getWidth(), static_cast<int>(contents.size() * localBounds.getHeight()) };
    g.setColour(colour_constants::backGround);
    g.drawRect(bounds);
}

void DropDownMenu::resized()
{
}

void DropDownMenu::addToContents(String entryToAdd)
{
    contents.push_back(entryToAdd);
}

void DropDownMenu::setCurrentIndex(size_t index)
{
    currentIndex = index;
}
