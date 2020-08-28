/*
   ==============================================================================

   GraphDisplay.h
Created: 3 Mar 2020 11:27:17pm
Author:  Danny Herbert

==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "Graph.h"

class GraphDisplay : public Component,
	public Timer
{
public:
	static const int RING_BUFFER_SIZE = 300; // 300 is highest common factor of all sample rates. 

	GraphDisplay(Graph* newGraph);
	~GraphDisplay();

	void paint(Graphics&) override;
	void timerCallback() override;
	void resized() override;

private:
	Graph* graph;
	std::vector<float> displayVector;
	CustomLookAndFeel2 lookAndFeel2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphDisplay)
};
