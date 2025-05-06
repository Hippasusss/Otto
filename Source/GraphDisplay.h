/*
   ==============================================================================

   GraphDisplay.h
Created: 3 Mar 2020 11:27:17pm
Author:  Danny Herbert

==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EnvelopeFollower.h"
#include "Graph.h"
#include "DisplayData.h"

class GraphDisplay : public Component,
	public Timer
{
public:
	GraphDisplay(Graph* newGraph, EnvelopeFollower* newEnvelopeFollower);
	~GraphDisplay();

    void drawPath(Graphics& graphics, const DisplayData<float>& data, 
                           Colour colour, bool shouldFill, float strokeThickness);

	void paint(Graphics&) override;
	void timerCallback() override;
	void resized() override;

private:
	Graph* graph;
	EnvelopeFollower* envelopeFollower;
    DisplayData<float> audioDisplayData;
    DisplayData<float> envelopeDisplayData;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphDisplay)
};
