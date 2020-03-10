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

    bool operator>(ValueType);
    bool operator<(ValueType);
    bool operator>(const LevelValue& other);
    bool operator<(const LevelValue& other);
    ValueType operator=(const LevelValue& other);
    ValueType operator=(ValueType);
    ValueType operator-(ValueType);
    ValueType operator+(ValueType);

    ValueType getValue() const;
    ValueType getValueUnitDB() const;
    ValueType getValueDBFS() const;
    ValueType getSmoothedValue() const;
    ValueType getSmoothedValueUnitDB() const;
    ValueType getSmoothedValueDBFS() const;
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
LevelValue<ValueType>::~LevelValue()
{
}

template <typename ValueType>
bool LevelValue<ValueType>::operator>(ValueType other)
{
    return value > other;
}

template <typename ValueType>
bool LevelValue<ValueType>::operator<(ValueType other)
{
    return value < other;
}

template <typename ValueType>
bool LevelValue<ValueType>::operator>(const LevelValue& other)
{
    return value > other.value;
}

template <typename ValueType>
bool LevelValue<ValueType>::operator<(const LevelValue& other)
{
    return value < other.value;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::operator=(const LevelValue& other)
{
    setValue(other.value);
    return value;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::operator=(ValueType other)
{
    setValue(other);
    return value;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::operator-(ValueType other)
{
    return value - other;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::operator+(ValueType other)
{
    return value + other;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getValue() const
{
    return value;
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getValueUnitDB() const
{
	return Helpers::getUnitDB(value, dbMinusInfinity);
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
ValueType LevelValue<ValueType>::getSmoothedValueUnitDB() const
{
	return Helpers::getUnitDB(smoother.getValue(), dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValue<ValueType>::getSmoothedValueDBFS() const
{
    return Decibels::gainToDecibels<ValueType>(smoother.getValue(), -dbMinusInfinity);
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
