/*
   ==============================================================================

   EnvelopeFollower.cpp
Created: 22 Feb 2020 2:39:50pm
Author:  Danny Herbert

==============================================================================
*/

#include <JuceHeader.h>
#include "EnvelopeFollower.h"

EnvelopeFollower::EnvelopeFollower(): 
    envelopeDisplayData{44100},
    sampleRate(44100),
    numChannels(2),
    maxBlockSize(0),
    amount(1),
    attackTime(10.0f),
    releaseTime(10.0f),
    envelopeOutput{},
    envelopeState{}
{
    setAttack(attackTime);
    setRelease(releaseTime);
}

EnvelopeFollower::~EnvelopeFollower() = default;

void EnvelopeFollower::prepare(const dsp::ProcessSpec& spec) 
{
    numChannels = spec.numChannels;
    sampleRate = spec.sampleRate;
    maxBlockSize = spec.maximumBlockSize;
    envelopeOutput.resize(maxBlockSize);
    envelopeState.resize(numChannels);
    setAttack(attackTime);
    setRelease(releaseTime);
    envelopeDisplayData.resize(sampleRate);
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
    const auto& inputBlock = context.getInputBlock();
    const size_t numSamples = inputBlock.getNumSamples();

    for(auto channelIndex = 0; channelIndex < numChannels; ++channelIndex)
    {
        for(auto sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
        {
            const float sample = std::fabs(inputBlock.getSample(channelIndex, sampleIndex));
            if (sample > envelopeState[channelIndex]) 
                envelopeState[channelIndex] = attackAlpha * envelopeState[channelIndex] + (1 - attackAlpha) * sample;
            else
                envelopeState[channelIndex] = releaseAlpha * envelopeState[channelIndex] + (1 - releaseAlpha) * sample;

            envelopeOutput[sampleIndex] = (channelIndex == 0) ? 
                envelopeState[channelIndex] * amount : 
                std::max(envelopeOutput[sampleIndex], envelopeState[channelIndex]) * amount;
        }
    }
    envelopeDisplayData.writeValues(envelopeOutput);
}

void EnvelopeFollower::reset()
{
}

void EnvelopeFollower::setAttack(const float milliseconds)
{
    attackTime = milliseconds;
    attackAlpha = exp(-1000.0f / (sampleRate * attackTime));

}

void EnvelopeFollower::setRelease(const float milliseconds)
{
    releaseTime = milliseconds;
    releaseAlpha = exp(-1000.0f / (sampleRate * releaseTime));
}

void EnvelopeFollower::setAmount(float newAmount)
{
    amount = newAmount;
}

const std::vector<float>& EnvelopeFollower::getEnvelope() const
{
    return envelopeOutput;
}

