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
    setAlwaysOnTop(true);
    setVisible(true);
}

void DropDownContext::paint (juce::Graphics& graphics) 
{
    const auto& localBounds = getLocalBounds();
    graphics.setColour(colour_constants::lightMain);
    graphics.drawRect(localBounds);
}

void DropDownContext::resized() 
{
    toFront(false);
    auto parentBounds = getTopLevelComponent()->getLocalArea(&parent, parent.getLocalBounds());
    Rectangle<int> fullBounds = {parentBounds.getX(), parentBounds.getHeight() + parentBounds.getY(), parentBounds.getWidth(), static_cast<int>(parentBounds.getHeight() * buttonEntries.size()) };
    setBounds(fullBounds);
    for (size_t i = 0; i < buttonEntries.size(); ++i) 
    {
        auto bounds = getLocalBounds();
        bounds.setHeight(parentBounds.getHeight());
        bounds.setY(parentBounds.getHeight() * i);
        buttonEntries[i]->setBounds(bounds);
    }
}

void DropDownContext::addEntry(const String& entryName)
{    
    auto newButton = std::make_unique<TextButton>(entryName);
    addAndMakeVisible(*newButton);
    buttonEntries.emplace_back(std::move(newButton));
    resized();
}

void DropDownContext::addEntries(std::initializer_list<juce::String> entries)
{
    buttonEntries.reserve(buttonEntries.size() + entries.size());  
    
    for (const auto& entry : entries)
    {
        auto& button = buttonEntries.emplace_back(std::make_unique<juce::TextButton>(entry));
        addAndMakeVisible(button.get());
    }
    
    resized();
}

// DROPDOWNMENU
//==============================================================================
DropDownMenu::DropDownMenu() : contextMenu(*this)
{
    init();
}
DropDownMenu::DropDownMenu(const String& name) : contextMenu(*this)
{
    init();
    setName(name);
}

void DropDownMenu::init()
{
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
}

void DropDownMenu::resized()
{
    Rectangle<int> bounds = getTopLevelComponent()->getLocalArea(this, getLocalBounds());
    bounds.setY(bounds.getY() + bounds.getHeight());
    contextMenu.setBounds(bounds);
}

void DropDownMenu::addToContents(const String& entryToAdd)
{
    if (getParentComponent() != nullptr) {
        getTopLevelComponent()->addAndMakeVisible(contextMenu);
    }
    contextMenu.addEntry(entryToAdd);
}

void DropDownMenu::setCurrentIndex(size_t index)
{
    currentIndex = index;
}
