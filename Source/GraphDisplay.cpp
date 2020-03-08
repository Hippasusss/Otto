/*
  ==============================================================================

    GraphDisplay.cpp
    Created: 3 Mar 2020 11:27:17pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GraphDisplay.h"

//==============================================================================

GraphDisplay::GraphDisplay()
{
    startTimerHz(60);
}
GraphDisplay::~GraphDisplay() = default;

void GraphDisplay::paint (Graphics& graphics)
{
    const int width = getBounds().getWidth();
    const int height = getBounds().getHeight();
    const int x = width/2;
    const int y = height/2;
    const int offset = 50;
    const float valuesSize = valuesToDraw.getSize();
    const NormalisableRange<float> range (0, 1, 0.001, 0.1, true);

    for(int i = 0; i < valuesSize; ++i)
    {
        const float value = range.convertFrom0to1(valuesToDraw.getValue(i));
        const float ellipseWidth = width * value + offset;
        const float ellipseHeight = height * value + offset;

        graphics.setColour(Colour::fromFloatRGBA(1, 0.4,0.3, i / valuesSize ));
        graphics.drawEllipse(x - ellipseWidth/2 , y - ellipseHeight/2, ellipseWidth, ellipseHeight, 2);
    }
}

void GraphDisplay::timerCallback()
{
    valuesToDraw.addValue(getValueCallback());
    repaint();
}

