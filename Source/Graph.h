/*
  ==============================================================================

    Graph.h
    Created: 12 Jul 2020 8:09:50pm
    Author:  Danny Herbert

  ==============================================================================
*/
#include "JuceHeader.h"
#include "RingBuffer.h"

#pragma once
class Graph : dsp::ProcessorBase
{
public:
    Graph() = default;
    ~Graph() = default;
	void prepare(const dsp::ProcessSpec&) override;
	void process(const dsp::ProcessContextReplacing<float>&) override;
	void reset() override;

    RingBufferAudio<float>& getBuffer();
    size_t getNumChannels() const;

private:
    RingBufferAudio<float> buffer;
    size_t numChannels;
};
