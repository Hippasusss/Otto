/*
  ==============================================================================

    Meter.cpp
    Created: 8 Mar 2020 7:51:43pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include "Meter.h"

#include "Helpers.h"

Meter::Meter()
{
}

Meter::~Meter()
{
}

void Meter::prepare(const dsp::ProcessSpec& spec)
{
    numChannels = spec.numChannels;
    RMS.resize(numChannels);
    peak.resize(numChannels);
    if(prepareCallback) prepareCallback();
}

void Meter::process(const dsp::ProcessContextReplacing<float>& context)
{
    const auto& block = context.getOutputBlock();
    calculateRMS(block);
    calculatePeak(block);
}

void Meter::reset()
{
    RMS.clear();
    peak.clear();
}

void Meter::clearClip()
{
    clip = false;
}

void Meter::calculateRMS(const dsp::AudioBlock<float>& block)
{
	
    const int blockSize = block.getNumSamples();

    for(int i = 0; i < numChannels; ++i)
    {
		float sum = 0.0f;
        const auto chan = block.getChannelPointer(i);
		for (int j = 0; j < blockSize; ++j)
		{
			const auto sample = chan[j];
			sum += sample * sample;
		}
		RMS[i] = static_cast<float>(std::sqrt (sum / blockSize));
    }

}

void Meter::calculatePeak(const dsp::AudioBlock<float>& block)
{
	
    const int numSamples = block.getNumSamples();

    for(int i = 0; i < numChannels; ++i)
    {
		float max = 0.0f;
        const auto data = block.getChannelPointer(i);
		for (int j = 0; j < numSamples; ++j)
		{
			const auto sample = abs(data[j]);
			max = sample > max ? sample : max;
		}
		peak[i] = max;
        clip = max >= 1.0f ? true : false;
    }
}

float Meter::getRMS(size_t channel) const
{
    jassert(channel < RMS.size());
    return RMS[channel];
}

float Meter::getPeak(size_t channel) const
{
    jassert(channel < peak.size());
    return peak[channel];
}

bool Meter::getClip() const
{
    return clip;
}

int Meter::getNumChannels() const
{
    return numChannels;
}
