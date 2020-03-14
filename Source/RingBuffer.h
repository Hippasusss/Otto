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
//TODO: heap allocate if needed?
template<typename arrayType, typename containerType>
class RingBuffer
{
public:
    RingBuffer();
	void addValue(arrayType);
    size_t getSize() const;
    arrayType getValue(size_t i) const;
    arrayType operator[](size_t i);
protected:
    containerType valueArray;
    size_t index;
    size_t size;
};

//===============================================================================

template <typename arrayType, typename containerType>
RingBuffer<arrayType, containerType>::RingBuffer(): index(0), size(0) 
{
}

template <typename arrayType, typename containerType>
void RingBuffer<arrayType, containerType>::addValue(arrayType value)
{
    valueArray[index] = value;
    ++index;
    index %= size;
}

template <typename arrayType,  typename containerType>
size_t RingBuffer<arrayType, containerType>::getSize() const
{
    return size;
}

template <typename arrayType, typename containerType>
arrayType RingBuffer<arrayType, containerType>::getValue(size_t i) const
{
    return valueArray[(i + index) % size];
}

template <typename valueType, typename containerType>
valueType RingBuffer<valueType, containerType>::operator[](size_t i)
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


