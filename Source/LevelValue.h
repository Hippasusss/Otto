/*
  ==============================================================================

    LevelValue.h
    Created: 9 Mar 2020 8:49:15pm
    Author:  Danny Herbert

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>
#include "ValueSmoother.h"
#include "Constants.h"
#include "Helpers.h"

template<typename ValueType>
class LevelValue : Timer
{
public:
	LevelValue();
	LevelValue(ValueType value);
	~LevelValue();

    LevelValue& operator=(const LevelValue& other);
    LevelValue& operator=(ValueType);
    operator ValueType&();

    ValueType getValue() const;
    ValueType getValueNormalisedDB() const;
    ValueType getValueDBFS() const;
    ValueType getSmoothedValue() const;
    ValueType getSmoothedValueNormalisedDB() const;
    ValueType getSmoothedValueDBFS() const;
    ValueSmoother<ValueType>& getValueSmoother();
    void setValue(ValueType newValue);

	void timerCallback() override;
private:
    ValueType value;
    ValueSmoother<ValueType> smoother;
    const float dbMinusInfinity = 60;
	
};

template <typename ValueType>
LevelValue<ValueType>::LevelValue() : value(0), smoother(0)
{
    startTimerHz(timer_constants::REFRESH_RATE);
}

template <typename ValueType>
LevelValue<ValueType>::LevelValue(ValueType value) : value(value), smoother(value)
{
    startTimerHz(timer_constants::REFRESH_RATE);
}

template <typename ValueType>
LevelValue<ValueType>::~LevelValue() = default;


template <typename ValueType>
LevelValue<ValueType>& LevelValue<ValueType>::operator=(const LevelValue& other)
{
    if(&other == this) return *this;
    setValue(other.value);
    return *this;
}

template <typename ValueType>
LevelValue<ValueType>& LevelValue<ValueType>::operator=(ValueType other)
{
    setValue(other);
    return *this;
}

template <typename ValueType>
LevelValue<ValueType>::operator ValueType&()
{
    return value;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getValue() const
{
    return value;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getValueNormalisedDB() const
{
	return Helpers::getNormalisedDB(value, dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getValueDBFS() const
{
    return Decibels::gainToDecibels(value, -dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getSmoothedValue() const
{
    return smoother.getValue();
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getSmoothedValueNormalisedDB() const
{
	return Helpers::getNormalisedDB(smoother.getValue(), dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getSmoothedValueDBFS() const
{
    return Decibels::gainToDecibels<ValueType>(smoother.getValue(), -dbMinusInfinity);
}

template <typename ValueType>
ValueSmoother<ValueType>& LevelValue<ValueType>::getValueSmoother() 
{
    return smoother;
}

template <typename ValueType>
void LevelValue<ValueType>::setValue(ValueType newValue)
{
    value = newValue;
    smoother = newValue;
}

template <typename ValueType>
void LevelValue<ValueType>::timerCallback()
{
    smoother.incrementValue();
}
