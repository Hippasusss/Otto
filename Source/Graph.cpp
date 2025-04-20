/*
  ==============================================================================

	Graph.cpp
	Created: 12 Jul 2020 8:09:50pm
	Author:  Danny Herbert

  ==============================================================================
*/

#include "Graph.h"


Graph::Graph() :  audioDisplayData(44100)
{
}

void Graph::prepare(const dsp::ProcessSpec& spec) 
{
	audioDisplayData.resize(spec.sampleRate); // get 1 second
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
    const dsp::AudioBlock<const float> sourceBlock = context.getInputBlock();
    const size_t numSamples = sourceBlock.getNumSamples();
    const size_t numChannels = sourceBlock.getNumChannels();

    for (size_t sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        float sum = 0.0f;
        for (size_t channelIndex = 0; channelIndex < numChannels; ++channelIndex)
        {
            sum += sourceBlock.getChannelPointer(channelIndex)[sampleIndex];
        }
        float average = sum / static_cast<float>(numChannels);
        audioDisplayData.writeValue(average);
    }
}

void Graph::reset()
{
}

