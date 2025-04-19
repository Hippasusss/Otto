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

GraphDisplay::GraphDisplay(Graph* newGraph, EnvelopeFollower* newEnvelopeFollower) : graph(newGraph), envelopeFollower(newEnvelopeFollower),  displayAudioVector(RING_BUFFER_SIZE, 0), displayEnvelopeVector(RING_BUFFER_SIZE, 0)
{
	startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay() = default;


void GraphDisplay::drawPath(Graphics& graphics, const std::vector<float>& data, 
                           Colour colour, bool shouldFill, float strokeThickness = 1.0f)
{
    graphics.setColour(colour);
    
    const int width = getBounds().getWidth();
    const int height = getBounds().getHeight();
    const int numPoints = data.size();
    const float segmentWidth = static_cast<float>(width) / numPoints;

    Path path;
    path.preallocateSpace(numPoints * 3 + (shouldFill ? 4 : 0));
    
    if (shouldFill) {
        path.startNewSubPath(0, height);
    } else {
        path.startNewSubPath(0, height - (data[0] * height));
    }

    for (int i = shouldFill ? 0 : 1; i < numPoints; i++) {
        path.lineTo(i * segmentWidth, height - (data[i] * height));
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
    drawPath(graphics, displayAudioVector, colour_constants::lightMain, true);
    drawPath(graphics, displayEnvelopeVector, colour_constants::red, false, 3.0f);
}

void GraphDisplay::resized()
{
}

void GraphDisplay::timerCallback()
{
	graph->fillVectorWithAudioDisplayData(displayAudioVector);
    envelopeFollower->fillVectorWithEnvelopeDisplayData(displayEnvelopeVector);
	repaint();
}


