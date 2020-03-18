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

template<typename ValueType, typename ContainerType>
class RingBuffer
{
public:
	virtual ~RingBuffer() = default;
	virtual void addValue(ValueType);
    virtual size_t getSize() const;
    virtual ValueType getValue(size_t i) const;
    virtual ValueType operator[](size_t i);
protected:
    RingBuffer();
	RingBuffer(size_t size);
    ContainerType valueArray;
    size_t index;
    size_t size;
};

//===============================================================================

template <typename ValueType, typename ContainerType>
RingBuffer<ValueType, ContainerType>::RingBuffer() :  index(0), size(0) {}

template <typename ValueType, typename ContainerType>
RingBuffer<ValueType, ContainerType>::RingBuffer(size_t size) : index(0), size(size) {}

template <typename ValueType, typename ContainerType>
void RingBuffer<ValueType, ContainerType>::addValue(ValueType value)
{
    valueArray[index] = value;
    ++index;
    index %= size;
}

template <typename ValueType,  typename ContainerType>
size_t RingBuffer<ValueType, ContainerType>::getSize() const
{
    return size;
}

template <typename ValueType, typename ContainerType>
ValueType RingBuffer<ValueType, ContainerType>::getValue(size_t i) const
{
    return valueArray[(i + index) % size];
}

template <typename ValueType, typename ContainerType>
ValueType RingBuffer<ValueType, ContainerType>::operator[](size_t i)
{
    return valueArray[i];
}


//===============================================================================
// Vector
//===============================================================================

template<typename ValueType>
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
RingBufferVector<ValueType>::RingBufferVector(size_t newSize) : RingBuffer(newSize)
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

template<typename ValueType, size_t Size>
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
template<typename SampleType>
class RingBufferAudio 
{
public:
    RingBufferAudio() = default;
    RingBufferAudio(size_t channels, size_t length);
    void resize(size_t channels, size_t length);
	void appendBlock(const dsp::AudioBlock<const SampleType>& newBlock);
	dsp::AudioBlock<SampleType> getBlock();
private:
	AudioBuffer<SampleType> aggregateBuffer;
    size_t size = 0;
    size_t index = 0;
};

template <typename SampleType>
RingBufferAudio<SampleType>::RingBufferAudio(size_t channels, size_t length) :
	aggregateBuffer(channels, length)
{
    this->size = length;
    this->index = 0;
}

template <typename SampleType>
void RingBufferAudio<SampleType>::resize(size_t channels, size_t length)
{
    aggregateBuffer.setSize(channels, length);
    this->size = length;
}

template <typename SampleType>
void RingBufferAudio<SampleType>::appendBlock(const dsp::AudioBlock<const SampleType>& newBlock)
{
    const auto numSamplesToCopy = newBlock.getNumSamples();
    const auto remainingSpace = aggregateBuffer.getNumSamples() - this->index;

    if(numSamplesToCopy > remainingSpace)
    {
        // Copy to end of block
        Helpers::copyAudioBlockIntoBuffer
    		(newBlock, aggregateBuffer, remainingSpace, 0, this->index);

        // Reset and copy remaining samples
        this->index = 0;
		Helpers::copyAudioBlockIntoBuffer
    		(newBlock, aggregateBuffer, numSamplesToCopy - remainingSpace, remainingSpace, this->index);
    	this->index = numSamplesToCopy - remainingSpace;
    }
    else
    {
        // Just copy whole block if room
		Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, numSamplesToCopy, 0, this->index);
        this->index += numSamplesToCopy % size;
    }
}

template <typename SampleType>
dsp::AudioBlock<SampleType> RingBufferAudio<SampleType>::getBlock() 
{
    return dsp::AudioBlock<SampleType>(aggregateBuffer);
}


