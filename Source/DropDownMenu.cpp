/*
  ==============================================================================

    DropDownMenu.cpp
    Created: 7 May 2025 3:43:27pm
    Author:  danny

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DropDownMenu.h"

// DROPDOWNCONTEXT
//==============================================================================

DropDownContext::DropDownContext(Component& parentDDMenu) : parent(parentDDMenu)
{
}

void DropDownContext::paint (juce::Graphics& graphics) 
{
    const auto& localBounds = getLocalBounds();
    graphics.setColour(colour_constants::lightMain);
    graphics.fillRect(localBounds);
}

void DropDownContext::resized() 
{
    for (size_t i = 0; i < buttonEntries.size(); ++i) 
    {
        auto bounds = getLocalBounds();
        bounds.setY(bounds.getY() + bounds.getHeight() * i);
        buttonEntries[i]->setBounds(bounds);
    }
}

void DropDownContext::addEntry(const String& entryName)
{    
    buttonEntries.emplace_back(std::make_unique<ToggleButton>(entryName));
    resized();
}

// DROPDOWNMENU
//==============================================================================
DropDownMenu::DropDownMenu() : contextMenu(*this)
{
    getTopLevelComponent()->addAndMakeVisible(contextMenu);
    resized();
}

void DropDownMenu::paint (juce::Graphics& graphics)
{

    const auto& localBounds = getLocalBounds();
    graphics.setColour(colour_constants::lightMain);
    graphics.fillRect(localBounds);

    graphics.setColour (colour_constants::backGround);
    graphics.drawRect (localBounds, 1);

    graphics.setColour (colour_constants::backGround);
    graphics.setFont (juce::FontOptions (14.0f));
    graphics.drawText (text, localBounds, juce::Justification::centred, true);  
    contextMenu.paint(graphics);
}

void DropDownMenu::resized()
{
    Rectangle<int> bounds = {0,0,100,100}; //getTopLevelComponent()->getLocalArea(this, getLocalBounds());
    bounds.setY(bounds.getY() + bounds.getHeight());
    contextMenu.setBounds(bounds);
}

void DropDownMenu::addToContents(const String& entryToAdd)
{
    contextMenu.addEntry(entryToAdd);
}

void DropDownMenu::setCurrentIndex(size_t index)
{
    currentIndex = index;
}
