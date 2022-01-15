/*
   ==============================================================================

   RingBuffer.h
Created: 4 Mar 2020 12:28:21am
Author:  Danny Herbert

==============================================================================
*/

//===============================================================================
// Base 
//===============================================================================
#pragma once
#include "Helpers.h"

template <typename ValueType, typename ContainerType>
class RingBuffer
{
public:
	virtual ~RingBuffer() = default;
	virtual void writeValue(ValueType);
	virtual ValueType readValue();
	virtual void readPreviousValues(ContainerType& values);

	virtual size_t getSize() const;
	virtual ValueType operator[](size_t i);
protected:
	RingBuffer();
	RingBuffer(size_t size);
	ContainerType valueArray;
	size_t writeIndex;
	size_t readIndex;
	size_t size;
};

//===============================================================================

template <typename ValueType, typename ContainerType>
RingBuffer<ValueType, ContainerType>::RingBuffer() : writeIndex(0), readIndex(0), size(0)
{
}

template <typename ValueType, typename ContainerType>
RingBuffer<ValueType, ContainerType>::RingBuffer(size_t size) : valueArray(size, 0), writeIndex(0), readIndex(0), size(size)
{
}

template <typename ValueType, typename ContainerType>
void RingBuffer<ValueType, ContainerType>::writeValue(ValueType value)
{
	valueArray[writeIndex] = value;
	++writeIndex %= size;
}

template <typename ValueType, typename ContainerType>
ValueType RingBuffer<ValueType, ContainerType>::readValue()
{
	const ValueType returnValue = valueArray[readIndex];
	++readIndex %= size;
	return returnValue;
}


template <typename ValueType, typename ContainerType>
size_t RingBuffer<ValueType, ContainerType>::getSize() const
{
	return size;
}


template <typename ValueType, typename ContainerType>
ValueType RingBuffer<ValueType, ContainerType>::operator[](size_t i)
{
	return valueArray[i];
}

// copies into provided array/container. copies as many as can into given size
// --------------------|---------------------------|-|----------
//                     ^ size             <i--- -1 ^ ^ write pointer
//                     |------This is copied-------|
//
template <typename ValueType, typename ContainerType>
void RingBuffer<ValueType, ContainerType>::readPreviousValues(ContainerType& values)
{
	const size_t inputSize = values.size();
	const size_t writeIndexLocal = writeIndex; // take local value in case class member is changed in separate thread.

	for (size_t i = 0; i < inputSize; i++)
	{
		// vile one liner takes care of wraparound of index when copying
		const size_t copyIndex = ((((writeIndexLocal - (i + 1)) % size) + size) % size);
		values[inputSize - 1 - i] = valueArray[copyIndex];
	}
}

//===============================================================================
// Vector
//===============================================================================

template <typename ValueType>
class RingBufferVector : public RingBuffer<ValueType, std::vector<ValueType>>
{
public:
	RingBufferVector();
	RingBufferVector(size_t newSize);
	void resize(size_t newSize);

};

//===============================================================================

template <typename ValueType>
RingBufferVector<ValueType>::RingBufferVector()
{
}

template <typename ValueType>
RingBufferVector<ValueType>::RingBufferVector(size_t newSize) : RingBuffer<ValueType, std::vector<ValueType>>(newSize)
{
	this->valueArray.resize(newSize);
}

template <typename ValueType>
void RingBufferVector<ValueType>::resize(size_t newSize)
{
	this->size = newSize;
	this->valueArray.resize(newSize);
}

//===============================================================================
// Array 
//===============================================================================

template <typename ValueType, size_t Size>
class RingBufferArray : public RingBuffer<ValueType, std::array<ValueType, Size>>
{
public:
	RingBufferArray();
};

template <typename ValueType, size_t Size>
RingBufferArray<ValueType, Size>::RingBufferArray()
{
	this->size = Size;
}

//===============================================================================
// Audio Block
//===============================================================================
template <typename SampleType>
class RingBufferAudio
{
public:
	RingBufferAudio() = default;
	RingBufferAudio(size_t channels, size_t length);
	void resize(size_t channels, size_t length);
	void writeBlock(const dsp::AudioBlock<const SampleType>& newBlock);
	void readBlock(AudioBuffer<SampleType>& bufferToFill);
	void getPreviousSamples(AudioBuffer<SampleType>& bufferToFill);
	dsp::AudioBlock<SampleType> getBlock();
	AudioBuffer<SampleType> getBuffer();
private:
	AudioBuffer<SampleType> aggregateBuffer;
	size_t numSamples = 0;
	size_t numChannels = 0;
	size_t writeIndex = 0;
	size_t readIndex = 0;
};

template <typename SampleType>
RingBufferAudio<SampleType>::RingBufferAudio(size_t channels, size_t length) :
	aggregateBuffer(channels, length), numSamples(length), numChannels(channels)
{
}

template <typename SampleType>
void RingBufferAudio<SampleType>::resize(size_t channels, size_t length)
{
	aggregateBuffer.setSize(channels, length);
	numSamples = length;
	numChannels = channels;
}

template <typename SampleType>
void RingBufferAudio<SampleType>::writeBlock(const dsp::AudioBlock<const SampleType>& newBlock)
{
	const size_t numSamplesToCopy = newBlock.getNumSamples();
	const size_t remainingSpace = numSamples - writeIndex;

	if(numSamplesToCopy > remainingSpace)
	{
		Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, remainingSpace, 0, writeIndex);
		Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, numSamplesToCopy - remainingSpace, remainingSpace, 0);
	}
	else
	{
		Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, numSamplesToCopy, 0, writeIndex);
	}

	writeIndex = (writeIndex + numSamplesToCopy) % numSamples;
}

template <typename SampleType>
void RingBufferAudio<SampleType>::readBlock(AudioBuffer<SampleType>& bufferToFill)
{
	const size_t difference = writeIndex - readIndex;
	const size_t remainingSpace = numSamples - readIndex;
	if (difference == 0) return;

	const size_t numSamplesToCopy = writeIndex > readIndex ? difference : remainingSpace + writeIndex;

	bufferToFill.setSize(aggregateBuffer.getNumChannels(), numSamplesToCopy, true, false, true);
	const dsp::AudioBlock<const SampleType> tempBlock = dsp::AudioBlock<SampleType>(aggregateBuffer);

	if(numSamplesToCopy <= remainingSpace)
	{
		Helpers::copyAudioBlockIntoBuffer(tempBlock, bufferToFill, numSamplesToCopy, readIndex);
	}
	else
	{
		Helpers::copyAudioBlockIntoBuffer(tempBlock, bufferToFill, remainingSpace, readIndex);
		Helpers::copyAudioBlockIntoBuffer(tempBlock, bufferToFill, numSamplesToCopy - remainingSpace, 0, remainingSpace);
	}

	readIndex = (readIndex + numSamplesToCopy) % numSamples;
}

template <typename SampleType>
void RingBufferAudio<SampleType>::getPreviousSamples(AudioBuffer<SampleType>& bufferToFill)
{
	const size_t numSamples = bufferToFill.getNumSamples();
	bufferToFill.setSize(numChannels, numSamples);
	const dsp::AudioBlock<const SampleType> tempBlock = dsp::AudioBlock<SampleType>(aggregateBuffer);

	if (writeIndex > numSamples)
	{
		Helpers::copyAudioBlockIntoBuffer(tempBlock, bufferToFill, numSamples, writeIndex - numSamples, 0);
	}
	else
	{
		Helpers::copyAudioBlockIntoBuffer(tempBlock, bufferToFill, writeIndex, 0, 0);
		const size_t samplesLeft = numSamples - writeIndex;
		Helpers::copyAudioBlockIntoBuffer(tempBlock, bufferToFill, samplesLeft, tempBlock.getNumSamples() - samplesLeft , writeIndex);
	}
	
}

template <typename SampleType>
dsp::AudioBlock<SampleType> RingBufferAudio<SampleType>::getBlock()
{
	return dsp::AudioBlock<SampleType>(aggregateBuffer);
}

template <typename SampleType>
AudioBuffer<SampleType> RingBufferAudio<SampleType>::getBuffer()
{
	return aggregateBuffer;
}
