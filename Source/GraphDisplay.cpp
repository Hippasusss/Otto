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

void GraphDisplay::paint(Graphics& graphics)
{
    // Draw Audio
    graphics.setColour(colour_constants::lightMain);

    const int width = getBounds().getWidth();
    const int height = getBounds().getHeight(); 
    const int numPointsInPathAudio = displayAudioVector.size();
    const float segmentWidthAudio = static_cast<float>(width) / numPointsInPathAudio;

    Path pathAudio = Path();
    pathAudio.preallocateSpace(numPointsInPathAudio * 3 + 3 + 1); // * 3 for each line segment, +3 to end path,  +1 to close path
    pathAudio.startNewSubPath(0, height);

    for (int i = 0; i < numPointsInPathAudio; i++)
    {
        pathAudio.lineTo(i * segmentWidthAudio, height - (displayAudioVector[i] * height) );
    }
    pathAudio.lineTo(width, height);

    pathAudio.closeSubPath();

    graphics.fillPath(pathAudio);

    // Draw Envelope
    graphics.setColour(colour_constants::red);

    const int numPointsInPathEnvelope = displayEnvelopeVector.size();
    const float segmentWidthEnvelope = static_cast<float>(width)/ numPointsInPathEnvelope;

    Path pathEnvelope = Path();
    pathEnvelope.preallocateSpace(numPointsInPathEnvelope * 3);
    pathEnvelope.startNewSubPath(0, height - (displayEnvelopeVector[0] * height));
    for (int i = 1; i < numPointsInPathEnvelope; i++)
    {
        pathEnvelope.lineTo(i * segmentWidthEnvelope, height - (displayEnvelopeVector[i] * height) );
    }

    PathStrokeType stroke(1.0f); 
    stroke.setEndStyle(PathStrokeType::rounded); 
    stroke.setStrokeThickness(3);
    graphics.strokePath(pathEnvelope, stroke);

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


