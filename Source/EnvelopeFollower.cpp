/*
  ==============================================================================

    EnvelopeFollower.cpp
    Created: 22 Feb 2020 2:39:50pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include "EnvelopeFollower.h"
#include "JuceHeader.h"

EnvelopeFollower::EnvelopeFollower(): 
    sampleRate(44100),
	numChannels(2),
	blockSize(0),
	blockTime(0),
	value(0),
	inputScaling(0),
    attackTime(0.5f),
	releaseTime(1.0f),
	smoothedValue(0)
{

}

EnvelopeFollower::~EnvelopeFollower() = default;

void EnvelopeFollower::prepare(const dsp::ProcessSpec& spec) 
{
    numChannels = spec.numChannels;
    sampleRate = spec.sampleRate;
    blockSize = spec.maximumBlockSize;
    blockTime = blockSize / sampleRate * 1000.0f; 
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
	float sum {0};
    const auto& block = context.getInputBlock();

    for(auto i = 0; i < numChannels; ++i)
    {
	    const auto chan = block.getChannelPointer(i);
        for(auto j = 0; i < blockSize; ++i)
        {
	        sum += abs(chan[j]);
        }
    }

	const float average = sum / (blockSize * numChannels);
    const float rate = average > value ? attackTime : releaseTime; 
    value += (blockTime * rate ) * (average - value);
}

void EnvelopeFollower::reset()
{
}

void EnvelopeFollower::setAttack(const float milliseconds)
{
    attackTime = milliseconds;
}

void EnvelopeFollower::setRelease(const float milliseconds)
{
    releaseTime = milliseconds;
}

void EnvelopeFollower::setAmount(float amount)
{
    inputScaling = amount;
}

float EnvelopeFollower::getValue() const
{
    return value;
}
