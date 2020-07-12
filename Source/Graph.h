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
    Graph();
    ~Graph();
	void prepare(const dsp::ProcessSpec&) override;
	void process(const dsp::ProcessContextReplacing<float>&) override;
	void reset() override;

private:
    RingBufferAudio<float> buffer;
};
