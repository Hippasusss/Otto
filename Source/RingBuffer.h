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
    ContainerType valueArray;
    size_t index;
    size_t size;
};

//===============================================================================

template <typename ValueType, typename ContainerType>
RingBuffer<ValueType, ContainerType>::RingBuffer(): index(0), size(0) 
{
}

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
RingBufferVector<ValueType>::RingBufferVector(size_t newSize)  
{
    //TODO: construct without resize
    resize(newSize);
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
template<typename ValueType>
class RingBufferAudioBlock 
{
public:
    RingBufferAudioBlock() = default;
    RingBufferAudioBlock(size_t channels, size_t length);
    void resize(size_t channels, size_t length);
	void addBlock(const dsp::AudioBlock<ValueType>& newBlock);
	const dsp::AudioBlock<ValueType>& getBlock();
private:
	AudioBuffer<ValueType> aggregateBuffer;
    size_t size =0;
    size_t index =0;
};

template <typename ValueType>
RingBufferAudioBlock<ValueType>::RingBufferAudioBlock(size_t channels, size_t length) :
	aggregateBuffer(channels, length)
{
    this->size = length;
    this->index = 0;
}

template <typename ValueType>
void RingBufferAudioBlock<ValueType>::resize(size_t channels, size_t length)
{
    aggregateBuffer.setSize(channels, length);
    this->size = length;
}

template <typename ValueType>
void RingBufferAudioBlock<ValueType>::addBlock(const dsp::AudioBlock<ValueType>& newBlock)
{
    const auto copySize = newBlock.getNumSamples();
    const auto remainingSpace = aggregateBuffer.getNumSamples() - this->index;
    if(copySize > remainingSpace)
    {
        //Copy to end of block
        Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, this->index, 0, remainingSpace);

        //Reset and copy remaining samples
        this->index = 0;
		Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, remainingSpace, this->index, copySize - remainingSpace);
    	this->index = copySize - remainingSpace;
    }
    else
    {
		Helpers::copyAudioBlockIntoBuffer(newBlock, aggregateBuffer, 0, this->index, copySize);
        this->index += copySize;
    }
}

template <typename ValueType>
const dsp::AudioBlock<ValueType>& RingBufferAudioBlock<ValueType>::getBlock() 
{
    return dsp::AudioBlock<ValueType>(aggregateBuffer);
}

