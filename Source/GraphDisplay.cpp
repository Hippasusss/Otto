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
    const NormalisableRange<float> range (0, 1, 0.001, 0.2, true);

    for(int i = 0; i < valuesSize; ++i)
    {
        const float value = range.convertFrom0to1(valuesToDraw.getValue(i));
        const float shapeWidth = width * value + offset;
        const float shapeHeight = height * value + offset;

        //graphics.setColour(Colour::fromFloatRGBA(1, 0.4,0.3, i / valuesSize ));
        graphics.setColour(Colours::lightcoral);
        graphics.drawRect(x - shapeWidth/2 , y - shapeHeight/2, shapeWidth, shapeHeight, 2.0f);
    }
}

void GraphDisplay::timerCallback()
{
    valuesToDraw.addValue(getValueCallback());
    valueOffset = getValueOffsetCallback();
    repaint();
}

