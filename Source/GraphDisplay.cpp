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

GraphDisplay::GraphDisplay(Graph* newGraph) : graph(newGraph), displayBuffer(1, 100), dataBuffer(2, 100)
{
    startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay()
{
}

void GraphDisplay::paint (Graphics& graphics)
{
}

void GraphDisplay::resized()
{
}

void GraphDisplay::pushIntoDisplayBuffer(AudioBuffer<float>& buffer)
{
    const size_t numSamples = buffer.getNumSamples();
    const size_t numChannels = buffer.getNumChannels();

    // Move samples along to make room
    displayBuffer.copyFrom(0, 0, displayBuffer, 0, numSamples, displayBuffer.getNumSamples());

    auto* const displayWrite = displayBuffer.getWritePointer(0);
	for(auto i = 0; i < numChannels; ++i)
	{
		const auto* const dataChannelRead = tempBuffer.getReadPointer(i);
        for(auto j = 0; j < numSamples; ++j)
        {
	        displayWrite[j] += dataChannelRead[j];
        }
	}

    for (auto i = 0; i < numSamples; ++i)
    {
        displayWrite[i] = displayWrite[i] / numChannels;
        //displayWrite[i] = 0.5f;
    }
}

void GraphDisplay::timerCallback()
{
    const size_t numChannels = graph->getNumChannels();
    AudioBuffer<float> tempBuffer = AudioBuffer<float>();
    graph->getBuffer().readBlock(tempBuffer);

    const size_t numSamples = tempBuffer.getNumSamples();

    displayBuffer.setSize(1, numSamples);

    // Sum channels in tempBuffer to mono for display buffer
    auto* const displayWrite = displayBuffer.getWritePointer(0);
	for(auto i = 0; i < numChannels; ++i)
	{
		const auto* const dataChannelRead = tempBuffer.getReadPointer(i);
        for(auto j = 0; j < numSamples; ++j)
        {
	        displayWrite[j] += dataChannelRead[j];
        }
	}

    for (auto i = 0; i < numSamples; ++i)
    {
        displayWrite[i] = displayWrite[i] / numChannels;
        //displayWrite[i] = 0.5f;
    }
    repaint();
}


