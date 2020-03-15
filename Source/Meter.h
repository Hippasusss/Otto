/*
  ==============================================================================

    Meter.h
    Created: 8 Mar 2020 7:51:43pm
    Author:  Danny Herbert

  ==============================================================================
*/
#pragma once
#pragma once
#include "JuceHeader.h"
#include "RingBuffer.h"

//TODO: implement ring buffer to make time constant regardless of block size
class Meter: dsp::ProcessorBase
{
public:
	Meter();
    ~Meter();

	void prepare(const dsp::ProcessSpec&) override;
	void process(const dsp::ProcessContextReplacing<float>&) override;
	void reset() override;


	float getRMS(size_t channel) const;
	float getPeak(size_t channel) const;
	bool getClip() const;
	void clearClip();

    int getNumChannels() const;

	std::function<void()> prepareCallback;

private:
	void calculateRMS(const dsp::AudioBlock<float>&);
	void calculatePeak(const dsp::AudioBlock<float>&);

    std::vector<float> channelRMSValues {0, 0};
    std::vector<float> channelPeakValues {0, 0};

	RingBufferAudioBlock<float> RMSAudioBuffer;
	const float RMSTime = 0.3;

    bool clip;
    int numChannels;
	
};

