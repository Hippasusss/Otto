/*
  ==============================================================================

    ValueSmoother.h
    Created: 9 Mar 2020 6:00:32pm
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once
template <typename ValueType>
class RampSmoother : Timer
{
public:
	RampSmoother() : value(0), destinationValue(0){}
	RampSmoother(ValueType initValue, int refreshRateHz) : value(initValue), destinationValue(initValue)
	{
		startTimerHz(refreshRateHz);
	}

	RampSmoother(ValueType initValue, int refreshRateHz, float ARTime) : value(initValue), destinationValue(initValue)
	{
		release = ARTime;
		attack = ARTime;
		startTimerHz(refreshRateHz);
	}

	RampSmoother& operator=(RampSmoother&& other) noexcept
	{
		if (this == &other)
			return other;
		return *this;
	}

	RampSmoother& operator=(const RampSmoother& other) noexcept
	{
		destinationValue = other.destinationValue;
		return *this;
	}
	RampSmoother& operator=(ValueType other) noexcept
	{
		destinationValue = other;
		return *this;
	}

	ValueType operator-(ValueType other) noexcept
	{
		return destinationValue - other;
	}

	ValueType operator+(ValueType other) noexcept
	{
		return destinationValue + other;
	}

	bool operator>(const RampSmoother& other) noexcept
	{
		return destinationValue > other.destinationValue;
	}
	bool operator<(const RampSmoother& other) noexcept
	{
		return destinationValue < other.destinationValue;
	}

	void setValue(ValueType val)
	{
		destinationValue = val;
	}
	
	void setAttack(float newRate)
	{
		attack = newRate / getTimerInterval();
	}

	void setRelease(float newRate)
	{
		release = newRate / getTimerInterval();
	}

	void setRefreshRate(int newRate)
	{
		stopTimer();
		startTimerHz(newRate);
	}

	ValueType getValue() const
	{
		return value;
	}

	void timerCallback() override
	{
		const float envelopeTime = destinationValue > value ? attack : release;
		const float timerInterval = static_cast<float>(getTimerInterval()) / 1000;
		const float delta = destinationValue - value;
		value += (delta) / (envelopeTime / timerInterval);
	}

private:
	std::atomic<ValueType> value;
	std::atomic<ValueType> destinationValue;
	float attack  = 0.5;
	float release = 0.5;
};

