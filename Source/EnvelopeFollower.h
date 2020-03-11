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
    std::function<void(float)> onValueCalculated;

private:
    double sampleRate;
    unsigned int numChannels;
    unsigned int maxBlockSize;
    float blockTime;

    std::atomic<float> value;
    float amount;
    float attackTime, releaseTime;


};

