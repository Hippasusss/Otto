/*
  ==============================================================================

    EnvelopeFollower.h
    Created: 22 Feb 2020 2:39:50pm
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class EnvelopeFollower: dsp::ProcessorBase
{

public:
    EnvelopeFollower();
    ~EnvelopeFollower();

    void process(const dsp::ProcessContextReplacing<float>&) override;
    void prepare(const dsp::ProcessSpec&) override;
    void reset() override;

    void setAttack(float milliseconds);
    void setRelease(float milliseconds);
    void setAmount(float amount);
    float getValue() const;

private:
    double sampleRate;
    int numChannels;
    int blockSize;
    float blockTime;

    float value;
    float inputScaling;
    float attackTime, releaseTime;

    SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedValue;

};

