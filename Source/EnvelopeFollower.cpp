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
	amount(0),
    attackTime(0.5f),
	releaseTime(1.0f),
    audioBuffer(numChannels, sampleRate),
    copyBuffer(numChannels, 0)
{
}

EnvelopeFollower::~EnvelopeFollower() = default;

void EnvelopeFollower::prepare(const dsp::ProcessSpec& spec) 
{
    numChannels = spec.numChannels;
    sampleRate = spec.sampleRate;
    maxBlockSize = spec.maximumBlockSize;
    audioBuffer.resize(numChannels, spec.sampleRate);

    copyBuffer.setSize(numChannels, maxBlockSize);
    
    filter.state = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 10);
    filter.prepare(spec);
}

void EnvelopeFollower::process(const dsp::ProcessContextReplacing<float>& context) 
{
	 // IIR Filter
     // get audio block copy
     // turn it into normal vector (sum channels)
     // filter it
     // put it in ring buffer

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

