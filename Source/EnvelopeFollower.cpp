/*
   ==============================================================================

   EnvelopeFollower.cpp
Created: 22 Feb 2020 2:39:50pm
Author:  Danny Herbert

==============================================================================
*/


#include "JuceHeader.h"
#include "EnvelopeFollower.h"
#include "Helpers.h"

EnvelopeFollower::EnvelopeFollower(): 
    sampleRate(44100),
    numChannels(2),
    maxBlockSize(0),
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
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{


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

