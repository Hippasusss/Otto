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
	RampSmoother(ValueType initValue, int refreshRateHz);
	RampSmoother(ValueType initValue, int refreshRateHz, float ARTime);
	RampSmoother& operator=(RampSmoother&& other) noexcept;
	RampSmoother& operator=(const RampSmoother& other) noexcept;
	RampSmoother& operator=(ValueType other) noexcept;
	ValueType operator-(ValueType other) noexcept;
	ValueType operator+(ValueType other) noexcept;
	operator ValueType&();
	bool operator>(const RampSmoother& other) noexcept;
	bool operator<(const RampSmoother& other) noexcept;

	void setValue(ValueType val);
	void setAttack(float newRate);
	void setRelease(float newRate);
	void setRefreshRate(int newRate);
	ValueType getValue() const;

	void timerCallback() override;

private:
	std::atomic<ValueType> value;
	std::atomic<ValueType> destinationValue;
	float attack  = 0.5;
	float release = 0.5;
};

template <typename ValueType>
RampSmoother<ValueType>::RampSmoother(ValueType initValue, int refreshRateHz): value(initValue),
                                                                               destinationValue(initValue)
{
	startTimerHz(refreshRateHz);
}

template <typename ValueType>
RampSmoother<ValueType>::RampSmoother(ValueType initValue, int refreshRateHz, float ARTime): value(initValue),
                                                                                             destinationValue(initValue)
{
	release = ARTime;
	attack = ARTime;
	startTimerHz(refreshRateHz);
}

template <typename ValueType>
RampSmoother<ValueType>& RampSmoother<ValueType>::operator=(RampSmoother&& other) noexcept
{
	if (this == &other)
		return other;
	return *this;
}

template <typename ValueType>
RampSmoother<ValueType>& RampSmoother<ValueType>::operator=(const RampSmoother& other) noexcept
{
	destinationValue = other.destinationValue;
	return *this;
}

template <typename ValueType>
RampSmoother<ValueType>& RampSmoother<ValueType>::operator=(ValueType other) noexcept
{
	destinationValue = other;
	return *this;
}

template <typename ValueType>
ValueType RampSmoother<ValueType>::operator-(ValueType other) noexcept
{
	return destinationValue - other;
}

template <typename ValueType>
ValueType RampSmoother<ValueType>::operator+(ValueType other) noexcept
{
	return destinationValue + other;
}

template <typename ValueType>
RampSmoother<ValueType>::operator ValueType&()
{
	return value;
}

template <typename ValueType>
bool RampSmoother<ValueType>::operator>(const RampSmoother& other) noexcept
{
	return destinationValue > other.destinationValue;
}

template <typename ValueType>
bool RampSmoother<ValueType>::operator<(const RampSmoother& other) noexcept
{
	return destinationValue < other.destinationValue;
}

template <typename ValueType>
void RampSmoother<ValueType>::setValue(ValueType val)
{
	destinationValue = val;
}

template <typename ValueType>
void RampSmoother<ValueType>::setAttack(float newRate)
{
	attack = newRate / getTimerInterval();
}

template <typename ValueType>
void RampSmoother<ValueType>::setRelease(float newRate)
{
	release = newRate / getTimerInterval();
}

template <typename ValueType>
void RampSmoother<ValueType>::setRefreshRate(int newRate)
{
	stopTimer();
	startTimerHz(newRate);
}

template <typename ValueType>
ValueType RampSmoother<ValueType>::getValue() const
{
	return value;
}

template <typename ValueType>
void RampSmoother<ValueType>::timerCallback()
{
	const float envelopeTime = destinationValue > value ? attack : release;
	const float timerInterval = static_cast<float>(getTimerInterval()) / 1000.0f;
	const float delta = destinationValue - value;
	value += delta / (envelopeTime / timerInterval);
}

