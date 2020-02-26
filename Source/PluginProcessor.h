#pragma once
#include <JuceHeader.h>
#include "EnvelopeFollower.h"

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
private:
    enum processors
    {
        inputGainIndex = 0,
        followerIndex,
        filterIndex,
        outputGainIndex
    };

	void setParameters();

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


    juce::dsp::ProcessorChain <dsp::Gain<float>, EnvelopeFollower,  dsp::LadderFilter<float>, dsp::Gain<float>> chain;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Auto_AudioProcessor)
};
