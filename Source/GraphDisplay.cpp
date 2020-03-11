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

GraphDisplay::GraphDisplay() : valuesToDraw(), valueOffset(0) 
{
    startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay() = default;

void GraphDisplay::paint (Graphics& graphics)
{
    const int width = getBounds().getWidth();
    const int height = getBounds().getHeight();
    const int x = width/2;
    const int y = height/2;
    const int offset = valueOffset; 
    const float valuesSize = valuesToDraw.getSize();

    for(int i = 0; i < valuesSize; ++i)
    {
        const float value = valuesToDraw.getValue(i) + valueOffset;
        const float shapeWidth = width * value + offset;
        const float shapeHeight = height * value + offset;

        graphics.setColour(Colours::lightcoral);
        graphics.drawRect(x - shapeWidth/2 , y - shapeHeight/2, shapeWidth, shapeHeight, 2.0f);
    }
}

void GraphDisplay::timerCallback()
{
    valuesToDraw.addValue(onAddValue());
    valueOffset = onSetValueOffset();
    repaint();
}

