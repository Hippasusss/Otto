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

	numSamplesToAverage = 300; // highest common factor of most common sample rates
	displayData.resize(sampleRate);
	sumBuffer.setSize(numChannels, numSamplesToAverage);
	sumBuffer.clear();
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
	static size_t sumIndex = 0;

	const dsp::AudioBlock<const float>sourceBlock = context.getInputBlock();
	const size_t inputSize = sourceBlock.getNumSamples();

	// for all channels
	for (size_t channel = 0; channel < sourceBlock.getNumChannels(); ++channel)
	{
		const auto* channelPointer = sourceBlock.getChannelPointer(channel);
		size_t inputIndex = 0;

		// do while there are still samples in the context buffer passed to this process 
		// sum a batch of input samples. stop if run out of input samples
		while (sumIndex < numSamplesToAverage && inputIndex < inputSize)
		{
			sumBuffer.setSample(channel, sumIndex, channelPointer[inputIndex]);
			inputIndex++;
			sumIndex++;
		}
		inputIndex = 0;
		sumIndex = 0;
	}

	// average the sum and add it to the display data if the sum buffer is full
	const float average = Helpers::getAverageMagnitude(sumBuffer);
	displayData.writeValue(average);

}

void Graph::reset()
{
}

void Graph::fillVectorWithDisplayData(std::vector<float>& data)
{
	displayData.readPreviousValues(data);
}
