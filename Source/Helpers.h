/*
  ==============================================================================

    Helpers.h
    Created: 9 Mar 2020 11:51:32am
    Author:  Danny Herbert

  ==============================================================================
*/
#include "JuceHeader.h"

#pragma once
static class Helpers
{
public:
    template<typename SampleType>
	static SampleType getAverageValue(const dsp::AudioBlock<SampleType>& block)
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
	//TODO: make pass through 0
	template<typename ValueType>
	static ValueType getUnitDB(ValueType value, ValueType dbMinusInfinity = -100)
    {
	    return jlimit<ValueType>(0.0f,1.0f,((ValueType(20.0) * std::log10f(value))/ ValueType(dbMinusInfinity)) + 1);
    }
};

