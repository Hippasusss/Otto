#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

#include "EnvelopeFollower.h"
#include "BufferStore.h"
#include "FilterFollower.h"
#include "Graph.h"
#include "Meter.h"
#include "Mixer.h"

class Auto_AudioProcessor: public AudioProcessor
{
public:
    Auto_AudioProcessor();
    ~Auto_AudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    EnvelopeFollower* getEnvelopeFollower();
    FilterFollower<float>* getLadderFilter();
    Meter* getInputMeter();
    Meter* getOutputMeter();
    Graph* getGraph();
    dsp::Oversampling<float>* getOversampling();
    void changeOversampling(size_t factor);
    dsp::ProcessSpec spec = dsp::ProcessSpec {0,0,0};

    static AudioProcessorValueTreeState::ParameterLayout getParameterLayout();
    AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", getParameterLayout()};
    void updateAllParameters();

private:
    enum inputProcessors
    {
        inputGainIndex,
        bufferStoreIndex,
        inputMeterIndex,
        graphIndex,
    };

    enum mainProcessors
    {
        followerIndex,
        filterIndex,
    };
    enum outputProcessors
    {
        mixerIndex,
        outputGainIndex,
        outputMeterIndex
    };

    dsp::Oversampling<float> oversamplers[4] =  
    {
        dsp::Oversampling<float>(2, 1, dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple),
        dsp::Oversampling<float>(2, 2, dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple),
        dsp::Oversampling<float>(2, 3, dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple),
        dsp::Oversampling<float>(2, 4, dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple),
    };

    dsp::Oversampling<float>* currentOversampler = &oversamplers[0];

    dsp::ProcessorChain <dsp::Gain<float>,
                         BufferStore,
                         Meter, 
                         Graph> inputChain;

    dsp::ProcessorChain <EnvelopeFollower,
                         FilterFollower<float>> chain;

    dsp::ProcessorChain <Mixer,
                         dsp::Gain<float>,
                         Meter> outputChain;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Auto_AudioProcessor)
};
