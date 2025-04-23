/*
  ==============================================================================

    Graph.h
    Created: 12 Jul 2020 8:09:50pm
    Author:  Danny Herbert

  ==============================================================================
*/
#pragma once
#include "RingBuffer.h"

class Graph : dsp::ProcessorBase
{
public:
    Graph();
    ~Graph() = default;
	void prepare(const dsp::ProcessSpec&) override;
	void process(const dsp::ProcessContextReplacing<float>&) override;
	void reset() override;
    RingBufferVector<float> audioDisplayData;
};
