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
    DisplayData(RingBufferVector<ValueType>& sourceBuffer, bool dbScale = false ,size_t lengthMs = 2600 ) : 
        sourceBuffer(sourceBuffer),
        reductionFactor(500),
        bufferSize(calculateBufferSize(lengthMs)),
        historyBuffer(bufferSize),
        dbScale(dbScale),
        sampleRate(sourceBuffer.getSize()),
        lengthMs(lengthMs)
    {
    }

    const size_t calculateBufferSize(size_t ms) const { return ((sourceBuffer.getSize()/ 1000) * ms) / reductionFactor; } ;

    void prepare()
    {
        sampleRate = sourceBuffer.getSize();
        bufferSize = calculateBufferSize(lengthMs);
        historyBuffer.resize(bufferSize);
        runningSum = 0;
        nextReadPosition = 0;
    }

    void updateValues() 
    {
        if (sourceBuffer.getSize() != sampleRate)
        {   
            prepare();
        }

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
    RingBufferVector<ValueType>& sourceBuffer;
    size_t reductionFactor;
    size_t bufferSize;
    RingBufferVector<ValueType> historyBuffer;
    size_t sampleRate;
    size_t lengthMs;
    bool dbScale = false;

    ValueType runningSum = 0;
    size_t nextReadPosition = 0;
};
