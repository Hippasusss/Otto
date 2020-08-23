/*
  ==============================================================================

	Graph.cpp
	Created: 12 Jul 2020 8:09:50pm
	Author:  Danny Herbert

  ==============================================================================
*/

#include "Graph.h"


void Graph::prepare(const dsp::ProcessSpec& spec)
{
	numChannels = spec.numChannels;
	sampleRate = spec.sampleRate;
	numSamplesToAverage = sampleRate / 300;
	sumBuffer.setSize(numChannels, numSamplesToAverage);
	displayData.resize(400);
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
	static size_t sumIndex = 0;

	const dsp::AudioBlock<const float>sourceBlock = context.getInputBlock();
	const size_t inputSize = sourceBlock.getNumSamples();
	size_t remainingSpace = numSamplesToAverage - sumIndex;

	for (size_t i = 0; i < sourceBlock.getNumChannels(); ++i)
	{
		const auto* channelPointer = sourceBlock.getChannelPointer(i);
		size_t inputIndex = 0;
		while (inputIndex < inputSize)
		{
			while (sumIndex < numSamplesToAverage)
			{
				sumBuffer.addSample(0, sumIndex, channelPointer[inputIndex]);
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
