/*==============================================================================

   Meter.h
Created: 8 Mar 2020 7:51:43pm
Author:  Danny Herbert

==============================================================================
*/
#pragma once
#include "JuceHeader.h"
#include "RingBuffer.h"

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

        std::function<void()> onPrepareCallback;

    private:
        void calculateRMS(const dsp::AudioBlock<float>&);
        void calculatePeak(const dsp::AudioBlock<float>&);

        //TODO: Not currently atomic: make thread safe using ringbuffer
        //std::vector<RingBufferFloat> channelRMSValuess { RingBufferFloat(1024), RingBufferFloat(1024)}
        std::vector<float> channelRMSValues {0, 0};
        std::vector<float> channelPeakValues {0, 0};

        RingBufferAudio<float> RMSAudioBuffer;
        const float RMSTime = 0.3;

        bool clip {};
        int numChannels {};

};

