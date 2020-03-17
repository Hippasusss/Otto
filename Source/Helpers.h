/*
  ==============================================================================

    Helpers.h
    Created: 9 Mar 2020 11:51:32am
    Author:  Danny Herbert

  ==============================================================================
*/
#include "JuceHeader.h"

#pragma once
class Helpers
{
public:
	//returns the absolute average value of samples 
    template<typename SampleType>
	static SampleType getAverageMagnitude(const dsp::AudioBlock<SampleType>& block)
    {
	    
		float sum {0};
	    const auto numChannels = block.getNumChannels();
	    const auto blockSize = block.getNumSamples();

	    for(auto i = 0; i < numChannels; ++i)
	    {
		    const auto chan = block.getChannelPointer(i);
	        for(auto j = 0; j < blockSize; ++j)
	        {
		        sum += abs(chan[j]);
	        }
	    }

		return sum / (blockSize * numChannels);
    }

    template<typename SampleType>
	static SampleType getMagnitude(const dsp::AudioBlock<SampleType>& block)
    {
	    
		float value {};
	    const auto numChannels = block.getNumChannels();
	    const auto blockSize = block.getNumSamples();

	    for(auto i = 0; i < numChannels; ++i)
	    {
		    const auto chan = block.getChannelPointer(i);
	        for(auto j = 0; j < blockSize; ++j)
	        {
				const SampleType sample = abs(chan[j]);
		        value = sample > value ? sample : value;
	        }
	    }

		return value / (blockSize * numChannels);
    }
	// TODO: Create properly normalised dB range which passes through 0.
	// Wouldn't be properly logarithmic but would avoid random -inf valuePossible?
	template<typename ValueType>
	static ValueType getNormalisedDB(ValueType value, ValueType dbMinusInfinity = -100)
    {
	    return jlimit<ValueType>(0.0f,1.0f,((ValueType(20.0) * std::log10f(value))/ ValueType(dbMinusInfinity)) + 1);
    }

	template<typename ValueType>
	static void copyAudioBlockIntoBuffer(const dsp::AudioBlock<ValueType>& sourceBlock, 
													AudioBuffer<ValueType>& destinationBuffer, 
													size_t sourceStartSample, 
													size_t destStartSample, 
													size_t numSamples)
	{
	    jassert(sourceBlock.getNumChannels() == destinationBuffer.getNumChannels());
		for(auto channel = 0; channel < destinationBuffer.getNumChannels(); ++channel)
		{
			auto* channelPointer = sourceBlock.getChannelPointer(channel);
			channelPointer += sourceStartSample;
			destinationBuffer.copyFrom(channel, destStartSample, channelPointer, numSamples);
		}
	}
private:
	Helpers() = default;
};

