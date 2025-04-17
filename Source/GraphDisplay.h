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
#include "EnvelopeFollower.h"
#include "Graph.h"

class GraphDisplay : public Component,
	public Timer
{
public:
	static const int RING_BUFFER_SIZE = 10000;

	GraphDisplay(Graph* newGraph, EnvelopeFollower* newEnvelopeFollower);
	~GraphDisplay();

	void paint(Graphics&) override;
	void timerCallback() override;
	void resized() override;

private:
	Graph* graph;
	EnvelopeFollower* envelopeFollower;
	std::vector<float> displayAudioVector;
	std::vector<float> displayEnvelopeVector;
    RingBufferVector<float> displayAudioRB;
    RingBufferVector<float> displayEnvelopeRB;
	CustomLookAndFeel2 lookAndFeel2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphDisplay)
};
