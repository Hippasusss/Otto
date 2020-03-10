/*
  ==============================================================================

    EnvelopeFollower.cpp
    Created: 22 Feb 2020 2:39:50pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include "EnvelopeFollower.h"
#include "JuceHeader.h"
#include "Helpers.h"

EnvelopeFollower::EnvelopeFollower(): 
    sampleRate(44100),
	numChannels(2),
	maxBlockSize(0),
	blockTime(0),
	value(0),
	inputScaling(0),
    attackTime(0.5f),
	releaseTime(1.0f)
{

}

EnvelopeFollower::~EnvelopeFollower() = default;

void EnvelopeFollower::prepare(const dsp::ProcessSpec& spec) 
{
    numChannels = spec.numChannels;
    sampleRate = spec.sampleRate;
    maxBlockSize = spec.maximumBlockSize;
    blockTime = maxBlockSize / sampleRate * 1000.0f; 
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
    const auto& block = context.getInputBlock();
	const float average = Helpers::getAverageValue(block);
    const float rate = average > value ? attackTime : releaseTime; 
    value += (blockTime / rate ) *  (average - value) * inputScaling;
    if(setParameterCallback != nullptr) setParameterCallback(value);
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
