#pragma once
#include <JuceHeader.h>
#include "EnvelopeFollower.h"
#include "BufferStore.h"
#include "Meter.h"
#include "Mixer.h"

class Auto_AudioProcessor: public AudioProcessor,
                           public Slider::Listener,
                           public Button::Listener
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

    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button*) override;
    const EnvelopeFollower& getEnvelopeFollower() const;
    const dsp::LadderFilter<float>& getLadderFilter() const;
    Meter* getInputMeter();
    Meter* getOutputMeter();



private:

    const float slow = 1.0;
    const float fast = 0.3;

    void setParameter(const String& parameterID);
    void initaliseParameters();

    enum processors
    {
        inputGainIndex,
        bufferStoreIndex,
        inputMeterIndex,
        followerIndex,
        filterIndex,
        mixerIndex,
        outputGainIndex,
        outputMeterIndex
    };

    AudioParameterFloat inputGain;
    AudioParameterFloat outputGain;
    AudioParameterFloat resonance;
    AudioParameterFloat frequency;
    AudioParameterFloat drive;
    AudioParameterFloat envAmount;
    AudioParameterFloat mix;
    AudioParameterBool envSpeed;
    AudioParameterBool twoFourPole;

    std::array<AudioParameterFloat*, 7> floatParameter {&inputGain, &outputGain, &resonance, 
                                                        &frequency, &drive, &envAmount, &mix};
    std::array<AudioParameterBool*, 2> boolParameter {&envSpeed, &twoFourPole};

    dsp::ProcessorChain <dsp::Gain<float>,
                         BufferStore,
						 Meter, 
                         EnvelopeFollower,
                         dsp::LadderFilter<float>,
                         Mixer,
                         dsp::Gain<float>,
						 Meter> chain;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Auto_AudioProcessor)
};
