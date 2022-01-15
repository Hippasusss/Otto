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

GraphDisplay::GraphDisplay(Graph* newGraph) : graph(newGraph), displayVector(RING_BUFFER_SIZE, 0) 
{
	startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay() = default;

void GraphDisplay::paint(Graphics& graphics)
{
	graphics.setColour(colour_constants::lightMain);

	const int width = getBounds().getWidth();
	const int height = getBounds().getHeight(); 
	const int numPointsInPath = displayVector.size();
	const float segmentWidth = static_cast<float>(width) / numPointsInPath;

	Path path = Path();
	path.preallocateSpace(numPointsInPath * 3 + 3 + 1); // * 3 for each line segment, +3 to end path,  +1 to close path
	path.startNewSubPath(0, height);

	for (int i = 0; i < numPointsInPath; i++)
	{
		const float value = 0;
		path.lineTo(i * segmentWidth, height - (displayVector[i] * height) );
	}
	path.lineTo(width, height);

	path.closeSubPath();

	graphics.fillPath(path);
}

void GraphDisplay::resized()
{
}

void GraphDisplay::timerCallback()
{
	graph->fillVectorWithDisplayData(displayVector);
	repaint();
}


