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
    DisplayData(RingBufferVector<ValueType>& sourceBuffer, bool dbScale = false ,size_t bufferSize = 600 ) : 
        sourceBuffer(sourceBuffer),
        historyBuffer(bufferSize),
        dbScale(dbScale),
        reductionFactor(bufferSize),
        bufferSize(bufferSize)
    {
    }

    void updateValues() 
    {
        if (reductionFactor == 0) return;

        auto values = sourceBuffer.readAllValues();
        const size_t sourceSize = values.size();

        if (sourceSize == 0) return;

        size_t count = 0;
        ValueType sum = runningSum;
        size_t pos = 0;

        while (pos < sourceSize)
        {
            sum += values[pos++];
            count++;

            if (count == reductionFactor || pos == sourceSize)
            {
                ValueType avg = sum / count;
                ValueType value = dbScale ? Helpers::getNormalisedDB(avg) : avg;
                historyBuffer.writeValue(value);
                sum = 0;
                count = 0;
            }
        }

        runningSum = sum;
        nextReadPosition = count;
    }
    

    std::vector<ValueType> getPreviousValues() const
    {
        std::vector<ValueType> returnVector(bufferSize);
        historyBuffer.readPreviousValues(returnVector);
        return returnVector;
    }

private:
    const size_t reductionFactor;
    const size_t bufferSize;
    ValueType runningSum = 0;
    size_t nextReadPosition = 0;
    bool dbScale = false;
    RingBufferVector<ValueType>& sourceBuffer;
    RingBufferVector<ValueType> historyBuffer;
};
