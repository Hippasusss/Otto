/*
   ==============================================================================

   LevelValue.h
Created: 9 Mar 2020 8:49:15pm
Author:  Danny Herbert

==============================================================================
*/


#pragma once
#include <JuceHeader.h>
#include "RampSmoother.h"
#include "Constants.h"
#include "Helpers.h"

template<typename ValueType>
class LevelValueDisplay 
{
    public:
        LevelValueDisplay();
        LevelValueDisplay(ValueType value);
        ~LevelValueDisplay();

        LevelValueDisplay& operator=(const LevelValueDisplay& other);
        LevelValueDisplay& operator=(ValueType);
        operator ValueType&();

        ValueType getValue() const;
        ValueType getValueNormalisedDB() const;
        ValueType getValueDBFS() const;
        ValueType getSmoothedValue() const;
        ValueType getSmoothedValueNormalisedDB() const;
        ValueType getSmoothedValueDBFS() const;
        void setValue(ValueType newValue);
        void setRate(float value);

    private:
        ValueType value;
        RampSmoother<ValueType> smoother;
        const float dbMinusInfinity = -60;
        const float AttackRelease = 0.2;

};

template <typename ValueType>
LevelValueDisplay<ValueType>::LevelValueDisplay() : value(0), smoother(0, timer_constants::REFRESH_RATE, AttackRelease)
{}

template <typename ValueType>
LevelValueDisplay<ValueType>::LevelValueDisplay(ValueType value) : value(value), smoother(value, timer_constants::REFRESH_RATE, AttackRelease)
{}

template <typename ValueType>
LevelValueDisplay<ValueType>::~LevelValueDisplay() = default;


template <typename ValueType>
LevelValueDisplay<ValueType>& LevelValueDisplay<ValueType>::operator=(const LevelValueDisplay& other)
{
    if(&other == this) return *this;
    setValue(other.value);
    return *this;
}

template <typename ValueType>
LevelValueDisplay<ValueType>& LevelValueDisplay<ValueType>::operator=(ValueType other)
{
    setValue(other);
    return *this;
}

template <typename ValueType>
LevelValueDisplay<ValueType>::operator ValueType&()
{
    return value;
}

template <typename ValueType>
ValueType LevelValueDisplay<ValueType>::getValue() const
{
    return value;
}

template <typename ValueType>
ValueType LevelValueDisplay<ValueType>::getValueNormalisedDB() const
{
    return Helpers::getNormalisedDB(value, dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValueDisplay<ValueType>::getValueDBFS() const
{
    return Decibels::gainToDecibels(value, dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValueDisplay<ValueType>::getSmoothedValue() const
{
    return smoother.getValue();
}

template <typename ValueType>
ValueType LevelValueDisplay<ValueType>::getSmoothedValueNormalisedDB() const
{
    return Helpers::getNormalisedDB(smoother.getValue(), dbMinusInfinity);
}

template <typename ValueType>
ValueType LevelValueDisplay<ValueType>::getSmoothedValueDBFS() const
{
    return Decibels::gainToDecibels<ValueType>(smoother.getValue(), dbMinusInfinity);
}

template <typename ValueType>
void LevelValueDisplay<ValueType>::setValue(ValueType newValue)
{
    value = newValue;
    smoother = newValue;
}

template <typename ValueType>
void LevelValueDisplay<ValueType>::setRate(float value)
{
    smoother.setAttack(value);
    smoother.setRelease(value);
}

