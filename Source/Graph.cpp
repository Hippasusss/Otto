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
    buffer.resize(spec.numChannels, spec.sampleRate); // take in a second worth of audio. ensures write doesnt catch read. 
}

void Graph::process(const dsp::ProcessContextReplacing<float>& context)
{
    buffer.writeBlock(context.getInputBlock());
}

void Graph::reset()
{
}
