/*
  ==============================================================================

    Mixer.cpp
    Created: 8 Mar 2020 11:49:53am
    Author:  Danny Herbert

  ==============================================================================
*/

#include "Mixer.h"
Mixer::Mixer() : otherBlock(nullptr), numChannels(0), maxBlockSize(0), mix(1)
{
}

Mixer::Mixer(dsp::AudioBlock<float>* other) : otherBlock(other), numChannels(0), maxBlockSize(0), mix(1)
{
}

void Mixer::prepare(const dsp::ProcessSpec& spec)
{
    numChannels = spec.numChannels;
    maxBlockSize = spec.maximumBlockSize;
}

void Mixer::process(const dsp::ProcessContextReplacing<float>& context)
{
    const dsp::AudioBlock<float>& outBlock =  context.getOutputBlock();
    const int blockSize = outBlock.getNumSamples();
    if(otherBlock == nullptr) return;

	for(auto i = 0; i < numChannels; ++i)
	{
        const auto outChan = outBlock.getChannelPointer(i);
        const auto otherChan = otherBlock->getChannelPointer(i);
		for(auto j = 0; j < blockSize; ++j)
		{
            outChan[j] = (outChan[j] * mix + otherChan[j] *(1 - mix));
		}
	}
}

void Mixer::reset()
{
}

void Mixer::setOtherBlock(dsp::AudioBlock<float>* other)
{
    otherBlock = other;
}

void Mixer::setMix(float value)
{
    mix = jlimit(0.0f, 1.0f,  value);
}
