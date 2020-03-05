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
GraphDisplay::GraphDisplay(const StackFIFO<float, 20>& fifoToDraw): valuesToDraw(fifoToDraw)
{
    startTimerHz(60);
}
GraphDisplay::~GraphDisplay() = default;

void GraphDisplay::paint (Graphics& graphics)
{
    const auto width = getBounds().getWidth();
    const auto height = getBounds().getHeight();
    const auto x = width/2;
    const auto y = height/2;
    const auto offset = 10;
    graphics.setColour(Colours::lightcoral);
    const float valuesSize = valuesToDraw.getSize();
    const NormalisableRange<float> range (0,1,0.01,0.8);

    for(int i = 0; i < valuesSize; ++i)
    {
        graphics.setColour(Colour::fromFloatRGBA(1, 0.4,0.3, i / valuesSize ));
        const float value = range.convertFrom0to1(valuesToDraw.getValue(i));
        const float ellipseWidth = width * value + offset;
        const float ellipseHeight = height * value + offset;
        graphics.drawEllipse(x - ellipseWidth/2 ,y - ellipseHeight/2, ellipseWidth, ellipseWidth, 3);
    }
}

void GraphDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GraphDisplay::updateListPointer(std::list<float>* list)
{
}

void GraphDisplay::timerCallback()
{
    repaint();
}
