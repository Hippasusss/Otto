/*
  ==============================================================================

	GraphDisplay.cpp
	Created: 3 Mar 2020 11:27:17pm
	Author:  Danny Herbert

  ==============================================================================
*/

#include "GraphDisplay.h"
#include "Constants.h"

//==============================================================================

GraphDisplay::GraphDisplay(Graph* newGraph, EnvelopeFollower* newEnvelopeFollower) :
    graph(newGraph),
    envelopeFollower(newEnvelopeFollower),
    audioDisplayData(newGraph->audioDisplayData, true),
    envelopeDisplayData(newEnvelopeFollower->envelopeDisplayData)
{
	startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay() = default;


void GraphDisplay::drawPath(Graphics& graphics, const DisplayData<float>& data, 
                           Colour colour, bool shouldFill, float strokeThickness = 1.0f)
{
    graphics.setColour(colour);
    
    auto previousValues = data.getPreviousValues();
    const int width = getBounds().getWidth();
    const int height = getBounds().getHeight();
    const int numPoints = previousValues.size();
    const float segmentWidth = static_cast<float>(width) / numPoints;

    Path path;
    path.preallocateSpace(numPoints * 3 + (shouldFill ? 4 : 0));
    
    if (shouldFill) {
        path.startNewSubPath(0, height);
    } else {
        path.startNewSubPath(0, height - (previousValues[0] * height));
    }

    for (int i = shouldFill ? 0 : 1; i < numPoints; i++) {
        path.lineTo(i * segmentWidth, height - (previousValues[i] * height));
    }

    if (shouldFill) {
        path.lineTo(width, height);
        path.closeSubPath();
        graphics.fillPath(path);
    } else {
        PathStrokeType stroke(strokeThickness);
        stroke.setEndStyle(PathStrokeType::rounded);
        graphics.strokePath(path, stroke);
    }
}

void GraphDisplay::paint(Graphics& graphics)
{
    drawPath(graphics, audioDisplayData, colour_constants::lightMain, true);
    drawPath(graphics, envelopeDisplayData, colour_constants::main, false, 3.0f);
}

void GraphDisplay::resized()
{
}

void GraphDisplay::timerCallback()
{
    envelopeDisplayData.updateValues();
    audioDisplayData.updateValues();
	repaint();
}






