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
    buffer.resize(spec.numChannels, spec.sampleRate * 1.5);  //1.5 gives extra space to write into to avoid the write pointer wrapping into the data being read. 
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
    buffer.writeBlock(context.getInputBlock());
}

void Graph::reset()
{
}

RingBufferAudio<float>& Graph::getBuffer()
{
    return buffer;
}

void Graph::fillVectorWithDisplayData(std::vector<float>& data)
{
	AudioBuffer<float> tempBuffer = AudioBuffer<float>(numChannels, sampleRate);
	buffer.getPreviousSamples(tempBuffer);
	const size_t numSamples = tempBuffer.getNumSamples();
	const size_t numData = data.size();

	Helpers::sumChannelsToFirstChannel(tempBuffer);

    const size_t samplesPerDatum = sampleRate/numData;
	const float* readPointer = tempBuffer.getReadPointer(0);
    for (int j = 0; j < numData; ++j)
    {

        float sum = 0;
        for(int i =0; i < samplesPerDatum; ++i)
        {
            const int offset = j * samplesPerDatum;
	        sum += readPointer[i + offset];
        }

        const float average = sum / samplesPerDatum;

        data[j] = Helpers::getNormalisedDB(average);
    }
}
