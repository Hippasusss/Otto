/*
  ==============================================================================

	Graph.cpp
	Created: 12 Jul 2020 8:09:50pm
	Author:  Danny Herbert

  ==============================================================================
*/

#include "Graph.h"


Graph::Graph() : displayData(44100), sumBuffer()
{
}

void Graph::prepare(const dsp::ProcessSpec& spec) 
{
	numChannels = spec.numChannels;
	sampleRate = spec.sampleRate;

	sumBufferSize = sampleRate / 150; // divides samplerates nicely 
	displayData.resize(sampleRate);
	sumBuffer.setSize(numChannels, sumBufferSize);
	sumBuffer.clear();
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
	static size_t sumIndex = 0;
	const dsp::AudioBlock<const float>sourceBlock = context.getInputBlock();
	const size_t numSamplesInput = sourceBlock.getNumSamples();
	size_t samplesRemaining = numSamplesInput;

	while (samplesRemaining > 0)
	{
		const size_t numWriteSamples = (samplesRemaining > sumBufferSize ? sumBufferSize : samplesRemaining) - sumIndex;
		// for all channels
		for (size_t channelIndex = 0; channelIndex < sourceBlock.getNumChannels(); ++channelIndex)
		{
			const auto* channelPointer = sourceBlock.getChannelPointer(channelIndex);
			sumBuffer.copyFrom(channelIndex, sumIndex, channelPointer, numWriteSamples);
		}
		sumIndex += numWriteSamples;
		sumIndex %= sumBufferSize;
		samplesRemaining -= numWriteSamples;
		if (sumIndex == 0)
		{
			auto value = Helpers::getNormalisedDB(Helpers::getAverageMagnitude(sumBuffer), -60.0f);
			displayData.writeValue(value);
		}
	}




}

void Graph::reset()
{
}

void Graph::fillVectorWithDisplayData(std::vector<float>& data)
{
	displayData.readPreviousValues(data);
}
