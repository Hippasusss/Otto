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

GraphDisplay::GraphDisplay(Graph* newGraph) : graph(newGraph), displayVector(300) // 300 is biggest factor of all sample rates. 
{
	startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay()
{
}

void GraphDisplay::paint(Graphics& graphics)
{
	graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));

	const int numPointsInPath = displayVector.size();
	const int segmentWidth = getBounds().getWidth() / numPointsInPath;
	Path path = Path();
	path.preallocateSpace(numPointsInPath * 3 + 1); // *3 for each line segment, +1 to close path
	path.startNewSubPath(0, 0);

	for (int i = 0; i < numPointsInPath; i++)
	{
		const float value = 0;
		path.lineTo(i * segmentWidth, 0);
	}

	path.closeSubPath();

	graphics.strokePath(path, PathStrokeType(2));
}

void GraphDisplay::resized()
{
}

void GraphDisplay::timerCallback()
{
	graph->fillVectorWithDisplayData(displayVector);
	repaint();
}


