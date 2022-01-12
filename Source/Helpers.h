/*
  ==============================================================================

    Helpers.h
    Created: 9 Mar 2020 11:51:32am
    Author:  Danny Herbert

  ==============================================================================
*/
#pragma once
#include "JuceHeader.h"

class Helpers
{
public:
	// Returns the absolute average value of samples from an AudioBlock or AudioBuffer
    template<typename SampleType> 
	static SampleType getAverageMagnitude(const dsp::AudioBlock<SampleType>& block)
    {
		SampleType sum {0};
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

	// ew
    template<typename SampleType> 
	static SampleType getAverageMagnitude(const AudioBuffer<SampleType>& block)
    {
		SampleType sum {0};
	    const auto numChannels = block.getNumChannels();
	    const auto blockSize = block.getNumSamples();

	    for(auto i = 0; i < numChannels; ++i)
	    {
		    const auto chan = block.getReadPointer(i);
	        for(auto j = 0; j < blockSize; ++j)
	        {
		        sum += abs(chan[j]);
	        }
	    }

		return sum / (blockSize * numChannels);
    }

	// Returns the highest sample magnitude of all channels
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

	// Copies the audio from the provided AudioBlock into the provided audio buffer
	template<typename SampleType>
	static void copyAudioBlockIntoBuffer(const dsp::AudioBlock<const SampleType>& sourceBlock,
	                                     AudioBuffer<SampleType>& destinationBuffer,
	                                     const size_t numSamples,
	                                     const size_t sourceStartSample = 0,
	                                     const size_t destStartSample = 0)
	{
	    jassert(sourceBlock.getNumChannels() == destinationBuffer.getNumChannels());
		for(auto channel = 0; channel < destinationBuffer.getNumChannels(); ++channel)
		{
			auto* channelPointer = sourceBlock.getChannelPointer(channel);
			channelPointer += sourceStartSample;
			destinationBuffer.copyFrom(channel, destStartSample, channelPointer, numSamples);
		}
	}

	template<typename SampleType>
	static void sumChannelsToFirstChannel(AudioBuffer<SampleType>& buffer)
    {
		const size_t numChannels = buffer.getNumChannels();
		const size_t numSamples = buffer.getNumSamples();
		auto* const dataChannelWrite = buffer.getWritePointer(0);
		for (size_t i = 1; i < numChannels; ++i)
		{
			const auto* const dataChannelRead = buffer.getReadPointer(i);
			for (size_t j = 0; j < numSamples; ++j)
			{
				dataChannelWrite[j] += dataChannelRead[j];
			}
		}

		// Average in place
		for (size_t i = 0; i < numSamples; ++i)
		{
			dataChannelWrite[i] /= numChannels;
		}
	}

	template<typename ValueType>
	static ValueType getNormalisedDB(ValueType value, ValueType dbMinusInfinity = -100)
    {
	    return jlimit<ValueType>(0.0f, 1.0f, ((ValueType(20.0) * std::log10f(abs(value)) / ValueType(-dbMinusInfinity)) + 1));
    }

private:
	Helpers() = default;
	~Helpers() = default;
};

