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
	amount(0),
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

// TODO: Implement proper envelope follower. Low oscillation for faster values.
void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
    const auto& block = context.getInputBlock();
	const float average = Helpers::getAverageMagnitude(block);
    value += ( average - value) * amount;
    //const bool attackRelease = average > value ? true : false; 
    //const float attackRelease = average > value ? attackTime: releaseTime; 
	//value += (blockTime / attackRelease) *  (average * amount - value) ;
	if(onValueCalculated) onValueCalculated(value);
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

void EnvelopeFollower::setAmount(float newAmount)
{
    amount = newAmount;
}

float EnvelopeFollower::getValue() const
{
    return value;
}
