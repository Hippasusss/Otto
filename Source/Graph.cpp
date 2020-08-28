/*
  ==============================================================================

	Graph.cpp
	Created: 12 Jul 2020 8:09:50pm
	Author:  Danny Herbert

  ==============================================================================
*/

#include "Graph.h"


Graph::Graph() : displayData(400), sumBuffer()
{
}

void Graph::prepare(const dsp::ProcessSpec& spec) 
{
	numChannels = spec.numChannels;
	sampleRate = spec.sampleRate;
	numSamplesToAverage = sampleRate / displayData.getSize() -100;
	sumBuffer.setSize(numChannels, numSamplesToAverage);
	sumBuffer.clear();
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
	static size_t sumIndex = 0;

	const dsp::AudioBlock<const float>sourceBlock = context.getInputBlock();
	const size_t inputSize = sourceBlock.getNumSamples();
	size_t remainingSpace = numSamplesToAverage - sumIndex;

	for (size_t channel = 0; channel < sourceBlock.getNumChannels(); ++channel)
	{
		const auto* channelPointer = sourceBlock.getChannelPointer(channel);
		size_t inputIndex = 0;
		while (inputIndex < inputSize)
		{
			while (sumIndex < numSamplesToAverage)
			{
				sumBuffer.setSample(channel, sumIndex, channelPointer[inputIndex]);
				inputIndex++;
				sumIndex++;
				if(inputIndex < inputSize) break;
			}
			if(sumIndex >= numSamplesToAverage)
			{
				const float average = Helpers::getAverageMagnitude(sumBuffer);
				displayData.writeValue(average);
				sumIndex = 0;
			}
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
