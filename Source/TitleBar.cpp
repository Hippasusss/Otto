/*
  ==============================================================================

    TitleBar.cpp
    Created: 13 Jan 2022 1:07:07am
    Author:  Danny Herbert

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TitleBar.h"

//==============================================================================
TitleBar::TitleBar()
{

}

TitleBar::~TitleBar()
{
}

void TitleBar::paint (juce::Graphics& g)
{
    auto background = getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId);
    auto foreground = getLookAndFeel().findColour(ResizableWindow::ColourIds::backgroundColourId);
    auto bounds = getLocalBounds();
    const Font font {"Futara", 23, Font::bold};
    const Font smallfont {"Futara", 14, Font::bold};

    g.fillAll (background);   // clear the background
    bounds.removeFromTop(3);
    bounds.removeFromLeft(20);

    g.setColour (foreground);
    g.setFont (font);
    g.drawText ("OTTO ", bounds.removeFromLeft(70),
                juce::Justification::bottomLeft, true);   // draw some placeholder text

    g.fillRect(bounds.removeFromLeft(3));
    bounds.removeFromLeft(10);

    g.setFont (smallfont);
    g.drawText ("HIPPASUS", bounds.removeFromLeft(100),
                juce::Justification::bottomLeft, true);   // draw some placeholder text
}

void TitleBar::resized()
{
    auto bounds = getLocalBounds();
    
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
