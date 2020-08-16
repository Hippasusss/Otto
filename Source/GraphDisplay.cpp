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

GraphDisplay::GraphDisplay(Graph* newGraph) : graph(newGraph), displayBuffer(1, 48000) 
{
    startTimerHz(timer_constants::REFRESH_RATE);
}
GraphDisplay::~GraphDisplay()
{
}

void GraphDisplay::paint (Graphics& graphics)
{
    graphics.setColour(getLookAndFeel().findColour(Slider::ColourIds::rotarySliderOutlineColourId));
}

void GraphDisplay::resized()
{
}

void GraphDisplay::pushIntoDisplayBuffer(AudioBuffer<float>& buffer)
{
    const size_t numSamples = buffer.getNumSamples();
    const size_t numChannels = buffer.getNumChannels();
    const int writeIndex = displayBuffer.getNumSamples() > numSamples ? displayBuffer.getNumSamples() - numSamples : 0;


    // TODO: Can't copy from to the same buffer
    // Move samples along to make room
    displayBuffer.copyFrom(0, 0, displayBuffer, 0, numSamples, displayBuffer.getNumSamples() - numSamples);

    // Sum data in place
	auto* const dataChannelWrite = buffer.getWritePointer(0);
	for(size_t i = 1; i < numChannels; ++i)
	{
		const auto* const dataChannelRead = buffer.getReadPointer(i);
        for(size_t j = 0; j < numSamples; ++j)
        {
	        dataChannelWrite[j] += dataChannelRead[j];
        }
	}

    // Average in place
    for (size_t i = 0; i < numSamples; ++i)
    {
        dataChannelWrite[i] /= numChannels;
    }

    // Copy into display buffer
	displayBuffer.copyFrom(0, writeIndex, buffer, 0, 0, numSamples);
}

void GraphDisplay::timerCallback()
{
    AudioBuffer<float> tempBuffer = AudioBuffer<float>();
    graph->getBuffer().getPreviousSamples(tempBuffer);

    const size_t numChannels = tempBuffer.getNumChannels();
    const size_t numSamples = tempBuffer.getNumSamples();
	auto* const dataChannelWrite = tempBuffer.getWritePointer(0);
	for(size_t i = 1; i < numChannels; ++i)
	{
		const auto* const dataChannelRead = tempBuffer.getReadPointer(i);
        for(size_t j = 0; j < numSamples; ++j)
        {
	        dataChannelWrite[j] += dataChannelRead[j];
        }
	}

    // Average in place
    for (size_t i = 0; i < numSamples; ++i)
    {
        dataChannelWrite[i] /= numChannels;
    }

	displayBuffer.copyFrom(0,0, tempBuffer, 0, 0, numSamples);
    //graph->getBuffer().readBlock(tempBuffer);
    //pushIntoDisplayBuffer(tempBuffer);

    repaint();
}


