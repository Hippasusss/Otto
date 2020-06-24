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
	amount(0),
	value(0, 1000),
    attackTime(0.5f),
	releaseTime(1.0f),
    audioBuffer(new RingBufferAudio<float>(numChannels, sampleRate)),
    copyBuffer(numChannels, 0)
{
}

EnvelopeFollower::~EnvelopeFollower() = default;

void EnvelopeFollower::prepare(const dsp::ProcessSpec& spec) 
{
    numChannels = spec.numChannels;
    sampleRate = spec.sampleRate;
    maxBlockSize = spec.maximumBlockSize;
    blockTime = maxBlockSize / sampleRate * 1000.0f; 
    audioBuffer->resize(numChannels, spec.sampleRate);

    value.setAttack(0.1);
    value.setRelease(0.7);

    copyBuffer.setSize(numChannels, maxBlockSize);
    
    filter.state = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 5);
    filter.prepare(spec);
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
    // Average EF (ringing at faster speeds, AR either equal or controlled by separate logic)
	 //   const int length = attackTime * sampleRate;
	 //   jassert(length <= sampleRate);

	 //   const dsp::AudioBlock<const float>& block = context.getInputBlock();
	 //   audioBuffer->appendBlock(block);

	//const float average = Helpers::getAverageMagnitude(audioBuffer->getBlock().getSubBlock(0, length));
	//   value = average * amount;
	//if(onValueCalculated) onValueCalculated(getValue());

	// IIR Filter
    const dsp::AudioBlock<const float>& block = context.getInputBlock();
    dsp::AudioBlock<float> copyBlock(copyBuffer);
    Helpers::copyAudioBlockIntoBuffer(block, copyBuffer, copyBuffer.getNumSamples());
    const dsp::ProcessContextReplacing<float> copyContext(copyBlock);
    filter.process(copyContext);
    auto max = copyContext.getOutputBlock().findMinAndMax().getEnd();
    onValueCalculated(max);

}

void EnvelopeFollower::reset()
{
    filter.reset();
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
    return value.getValue();
}

float EnvelopeFollower::getValueNormalisedDB() const
{
    return value.getValue();
}
