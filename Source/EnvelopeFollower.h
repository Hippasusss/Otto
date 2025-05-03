/*
   ==============================================================================

   EnvelopeFollower.h
Created: 22 Feb 2020 2:39:50pm
Author:  Danny Herbert

==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RingBuffer.h"

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
    void setAmount(float newAmount);
    float getSlowTime() {return slowTime;};
    float getFastTime() {return fastTime;};
    const std::vector<float>& getEnvelope() const;
    std::function<void(float)> onValueCalculated;
    RingBufferVector<float> envelopeDisplayData;

private:
    double sampleRate;
    unsigned int numChannels;
    unsigned int maxBlockSize;

    float amount;
    float attackTime, releaseTime;
    float attackAlpha, releaseAlpha;

    const float fastTime = 100;
    const float slowTime = 600;

    std::vector<float> envelopeOutput;
    std::vector<float> envelopeState;
};

