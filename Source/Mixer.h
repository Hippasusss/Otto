/*
  ==============================================================================

    Mixer.h
    Created: 8 Mar 2020 11:49:53am
    Author:  Danny Herbert

  ==============================================================================
*/
#include "JuceHeader.h"
#pragma once
class Mixer : dsp::ProcessorBase
{
public:
    Mixer();
    Mixer(dsp::AudioBlock<float>* other);
    void prepare(const dsp::ProcessSpec&) override;
    void process(const dsp::ProcessContextReplacing<float>&) override;
    void reset() override;
    void setOtherBlock(dsp::AudioBlock<float>* other);
    void setMix(float value);

private:
    dsp::AudioBlock<float>* otherBlock;
    int numChannels; 
    int maxBlockSize;
    float mix;

 };
