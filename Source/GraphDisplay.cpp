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

GraphDisplay::GraphDisplay(Graph* newGraph) : graph(newGraph), graphView(1), displayBuffer(1, 100)
{
    startTimerHz(timer_constants::REFRESH_RATE);

    //graphView.setLookAndFeel(&lookAndFeel2);
    graphView.setNumChannels(1);
    graphView.setBufferSize(1000);
    graphView.setColours(Colours::red, Colours::lightblue);
}
GraphDisplay::~GraphDisplay()
{
	graphView.setLookAndFeel(nullptr);
}

void GraphDisplay::paint (Graphics& graphics)
{
    const size_t numChannels = graph->getNumChannels();
    graph->getBuffer().readBlock(dataBuffer);

    const size_t numSamples = dataBuffer.getNumSamples();

    displayBuffer.setSize(1, numSamples);

    // Sum channels in tempBuffer to mono for display buffer
    const auto channelWrite = displayBuffer.getWritePointer(0);
	for(auto i = 0; i < numChannels; ++i)
	{
        const auto channel = dataBuffer.getReadPointer(i);
        for(auto j = 0; j < numSamples; ++j)
        {
	        channelWrite[j] += channel[j];
        }
	}

    for (auto i = 0; i < numSamples; ++i)
    {
        channelWrite[i] = channelWrite[i] / numChannels;
    }
    graphView.pushBuffer(displayBuffer);
}

void GraphDisplay::resized()
{
    graphView.clear();
    graphView.setBounds(getBounds());
}

void GraphDisplay::timerCallback()
{
    repaint();
}

