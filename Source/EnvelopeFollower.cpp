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
    releaseTime(1.0f),
    envelopeOutput(0)
{
}

EnvelopeFollower::~EnvelopeFollower() = default;

void EnvelopeFollower::prepare(const dsp::ProcessSpec& spec) 
{
    numChannels = spec.numChannels;
    sampleRate = spec.sampleRate;
    maxBlockSize = spec.maximumBlockSize;
    envelopeOutput.resize(maxBlockSize);
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
    const auto& inputBlock = context.getInputBlock();
    const size_t numSamples = inputBlock.getNumSamples();

    float env= 0;

    for(auto i = 0; i < numChannels; ++i)
    {
        auto channel = inputBlock.getSingleChannelBlock(i);
        for(auto j = 0; j < numSamples; ++j)
        {
            const float sample = abs(channel.getSample(0,j));
            if (sample > env) 
            {
                env = attackAlpha * env + (1 - attackAlpha) * sample;
            } 
            else 
            {
                env = releaseAlpha * env + (1 - releaseAlpha) * sample;
            }

            //TODO: not really average of channels for i>2
            envelopeOutput[j] += env;
        }
	if (numChannels > 1)
	{
	    for(auto& sample : envelopeOutput)
	    {
		sample/=numChannels;
	    }
	}
    }
}

void EnvelopeFollower::reset()
{
}

void EnvelopeFollower::setAttack(const float milliseconds)
{
    attackTime = milliseconds;
    attackAlpha = exp(-1.0f / (sampleRate * attackTime));

}

void EnvelopeFollower::setRelease(const float milliseconds)
{
    releaseTime = milliseconds;
    releaseAlpha = exp(-1.0f / (sampleRate * releaseTime));
}

void EnvelopeFollower::setAmount(float newAmount)
{
    amount = newAmount;
}


const std::vector<float>& EnvelopeFollower::getEnvelope()
{
    return envelopeOutput;
}

