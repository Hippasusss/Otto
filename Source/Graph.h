/*
  ==============================================================================

    Graph.h
    Created: 12 Jul 2020 8:09:50pm
    Author:  Danny Herbert

  ==============================================================================
*/
#pragma once
#include "JuceHeader.h"
#include "RingBuffer.h"

class Graph : dsp::ProcessorBase
{
public:
    Graph();
    ~Graph() = default;
	void prepare(const dsp::ProcessSpec&) override;
	void process(const dsp::ProcessContextReplacing<float>&) override;
	void reset() override;

    void fillVectorWithAudioDisplayData(std::vector<float>& data);
    void fillVectorWithEnvelopeDisplayData(std::vector<float>& data);

private:
    AudioBuffer<float> sumBuffer;
    RingBufferVector<float> audioDisplayData;
    RingBufferVector<float> envelopeDisplayData;
    size_t numChannels;
    size_t sampleRate;
    size_t sumBufferSize;
};
