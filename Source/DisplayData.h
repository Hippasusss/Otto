/*
  ==============================================================================

    DisplayData.h
    Created: 19 Apr 2025 4:12:38pm
    Author:  danny

  ==============================================================================
*/

#pragma once
#include "RingBuffer.h"

template <typename ValueType = float>
class DisplayData {
public:
    DisplayData(RingBufferVector<ValueType>& sourceBuffer, size_t bufferSize = 400)
        : sourceBuffer(sourceBuffer),
        historyBuffer(bufferSize)
    {
    }

    void updateValues() 
    {
        if (reductionFactor == 0) return;

        auto values = sourceBuffer.readAllValues();
        const size_t sourceSize = values.size();

        if (sourceSize == 0) return;

        for (size_t i = nextReadPosition; i < sourceSize; i += reductionFactor) 
        {
            historyBuffer.writeValue(values[i]);
        }

        if (sourceSize > nextReadPosition) 
        {
            nextReadPosition = reductionFactor - ((sourceSize - nextReadPosition) % reductionFactor);
        }
    }

    std::vector<ValueType> getPreviousValues() const
    {
        std::vector<ValueType> returnVector(400);
        historyBuffer.readPreviousValues(returnVector);
        return returnVector;
    }

private:
    const size_t reductionFactor = 100;
    size_t nextReadPosition = 0;
    RingBufferVector<ValueType>& sourceBuffer;
    RingBufferVector<ValueType> historyBuffer;
};
