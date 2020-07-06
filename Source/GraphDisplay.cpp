/*
  ==============================================================================

    GraphDisplay.cpp
    Created: 3 Mar 2020 11:27:17pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GraphDisplay.h"
#include "Constants.h"

//==============================================================================

GraphDisplay::GraphDisplay() : valuesToDraw(1, 512), valueOffset(0) , graph(1)
{
    startTimerHz(timer_constants::REFRESH_RATE);
    graph.setLookAndFeel(&lookAndFeel2);
}
GraphDisplay::~GraphDisplay()
{
	graph.setLookAndFeel(nullptr);
}

void GraphDisplay::paint (Graphics& graphics)
{
    const int width = getBounds().getWidth();
    const int height = getBounds().getHeight();
    const int x = width/2;
    const int y = height/2;
    const int offset = valueOffset; 
    graph.pushBuffer(valuesToDraw.getBuffer());

}

void GraphDisplay::resized()
{
    graph.clear();
}

void GraphDisplay::timerCallback()
{
    repaint();
}

