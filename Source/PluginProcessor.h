#pragma once
#include <JuceHeader.h>
#include "EnvelopeFollower.h"
#include "BufferStore.h"
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

    const EnvelopeFollower& getEnvelopeFollower() const;
    const dsp::LadderFilter<float>& getLadderFilter() const;
    Meter* getInputMeter();
    Meter* getOutputMeter();
    Graph* getGraph();

    static AudioProcessorValueTreeState::ParameterLayout getParameterLayout();
    AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", getParameterLayout()};
    float getAPVTSValue(const String& vlaue);
    void updateAllParameters();

private:

    const float slow = 1.0;
    const float fast = 0.3;

    enum processors
    {
        inputGainIndex,
        bufferStoreIndex,
        inputMeterIndex,
        graphIndex,
        followerIndex,
        filterIndex,
        mixerIndex,
        outputGainIndex,
        outputMeterIndex
    };


    dsp::ProcessorChain <dsp::Gain<float>,
                         BufferStore,
                         Meter, 
                         Graph,
                         EnvelopeFollower,
                         dsp::LadderFilter<float>,
                         Mixer,
                         dsp::Gain<float>,
                         Meter> chain;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Auto_AudioProcessor)
};
