/*
  ==============================================================================

    RingBuffer.h
    Created: 4 Mar 2020 12:28:21am
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once
//TODO: heap allocate if needed?
//TODO: Look into someone else's implementation of this. Should Read Backwards?
template<typename arrayType, int size>
class RingBuffer
{
public:
    RingBuffer();
	void addValue(arrayType);
    size_t getSize() const;
    arrayType getValue(size_t i) const;
    arrayType operator[](size_t i);
private:
	arrayType valueArray[size] {};
    int currentIndex;
    const int arraySize;
};

//===============================================================================

template <typename arrayType, int size>
RingBuffer<arrayType, size>::RingBuffer(): currentIndex(0), arraySize(size)
{
}

template <typename arrayType, int size>
void RingBuffer<arrayType, size>::addValue(arrayType value)
{
    valueArray[currentIndex] = value;
    ++currentIndex;
    currentIndex %= arraySize;
}

template <typename arrayType, int size>
size_t RingBuffer<arrayType, size>::getSize() const
{
    return arraySize;
}

template <typename arrayType, int size>
arrayType RingBuffer<arrayType, size>::getValue(size_t i) const
{
    return valueArray[(i + currentIndex) % arraySize];
}

template <typename arrayType, int size>
arrayType RingBuffer<arrayType, size>::operator[](size_t i)
{
    return valueArray[i];
}

