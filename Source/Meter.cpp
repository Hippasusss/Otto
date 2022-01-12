/*
   ==============================================================================

   Meter.cpp
Created: 8 Mar 2020 7:51:43pm
Author:  Danny Herbert

==============================================================================
*/

#include "Meter.h"

#include "Helpers.h"

Meter::Meter(): RMSAudioBuffer() {}
Meter::~Meter() = default;

void Meter::prepare(const dsp::ProcessSpec& spec)
{
    numChannels = spec.numChannels;
    channelRMSValues.resize(numChannels);
    channelPeakValues.resize(numChannels);
    RMSAudioBuffer.resize(numChannels, RMSTime * spec.sampleRate);
    if(onPrepareCallback) onPrepareCallback();
}

void Meter::process(const dsp::ProcessContextReplacing<float>& context)
{
    const auto& block = context.getOutputBlock();
    calculateRMS(block);
    calculatePeak(block);
}

void Meter::reset()
{
    channelRMSValues.clear();
    channelPeakValues.clear();
}

void Meter::clearClip()
{
    clip = false;
}

void Meter::calculateRMS(const dsp::AudioBlock<float>& block)
{
    RMSAudioBuffer.writeBlock(block);
    const auto RMSBlock = RMSAudioBuffer.getBlock(); // buffer is the length required for the RMS calc. Set in ctor
    const int blockSize = RMSBlock.getNumSamples();
    for(auto i = 0; i < numChannels; ++i)
    {
        float sum = 0.0f;
        const auto chan = RMSBlock.getChannelPointer(i);
        for (auto j = 0; j < blockSize; ++j)
        {
            const auto sample = chan[j];
            sum += sample * sample;
        }
        channelRMSValues[i] = (std::sqrt (sum / blockSize));
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
        channelPeakValues[i] = max;
        clip = max >= 1.0f ? true : false;
    }
}

float Meter::getRMS(size_t channel) const
{
    jassert(channel < channelRMSValues.size());
    return channelRMSValues[channel];
}

float Meter::getPeak(size_t channel) const
{
    jassert(channel < channelPeakValues.size());
    return channelPeakValues[channel];
}

bool Meter::getClip() const
{
    return clip;
}

int Meter::getNumChannels() const
{
    return numChannels;
}
