/*
  ==============================================================================

    RingBuffer.h
    Created: 4 Mar 2020 12:28:21am
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once
#include <array>
template<typename arrayType, int size>
class RingBuffer
{
public:
    RingBuffer();
	void addValue(arrayType);
    int getSize() const;
    arrayType getValue(int i) const;
    arrayType operator[](int i);
private:
	std::array<arrayType, size> valueArray {};
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
int RingBuffer<arrayType, size>::getSize() const
{
    return arraySize;
}

template <typename arrayType, int size>
arrayType RingBuffer<arrayType, size>::getValue(int i) const
{
    return valueArray[(i + currentIndex) % arraySize];
}

template <typename arrayType, int size>
arrayType RingBuffer<arrayType, size>::operator[](int i)
{
    return valueArray[i];
}

