/*
  ==============================================================================

    ValueSmoother.h
    Created: 9 Mar 2020 6:00:32pm
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once
template <typename ValueType>
class ValueSmoother
{
public:
	ValueSmoother() : value(0), destinationValue(0){}
	ValueSmoother(ValueType initValue) : value(initValue), destinationValue(initValue){}

	ValueSmoother& operator=(ValueSmoother&& other) noexcept
	{
		if (this == &other)
			return other;
		return *this;
	}

	ValueSmoother& operator=(const ValueSmoother& other)
	{
		destinationValue = other.destinationValue;
		return *this;
	}
	ValueType operator=(ValueType other) noexcept
	{
		destinationValue = other;
		return destinationValue;
	}

	ValueType operator-(ValueType other) noexcept
	{
		return destinationValue - other;
	}
	ValueType operator+(ValueType other) noexcept
	{
		return destinationValue + other;
	}

	bool operator>(const ValueSmoother& other) noexcept
	{
		return destinationValue > other.destinationValue;
	}
	bool operator<(const ValueSmoother& other) noexcept
	{
		return destinationValue < other.destinationValue;
	}

	void setValue(ValueType val)
	{
		destinationValue = val;
	}
	
	void setRate(float newRate)
	{
		rate = newRate;
	}

	ValueType getValue() const
	{
		return value;
	}

	void incrementValue()
	{
		value += (destinationValue - value) * rate;
	}

private:
	ValueType value;
	ValueType destinationValue;
	float rate = 0.5;
};
