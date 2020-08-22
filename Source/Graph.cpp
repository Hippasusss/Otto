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
    spareSamples.setSize(numChannels, spec.maximumBlockSize);
    numSamplesToAverage = sampleRate / 300;
    displayData.resize(400);
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
    const dsp::AudioBlock<const float>inputBuffer = context.getInputBlock();
    const size_t inputSize = inputBuffer.getNumSamples();
	
}

void Graph::reset()
{
}

void Graph::fillVectorWithDisplayData(std::vector<float>& data)
{
}
