#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Constants.h"

Auto_AudioProcessor::Auto_AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
            .withInput("Input", AudioChannelSet::stereo(), true)
#endif
            .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
            )
#endif
{
    chain.get<filterIndex>().setEnvFollowerPtr(&chain.get<followerIndex>());
}
Auto_AudioProcessor::~Auto_AudioProcessor() = default;

const String Auto_AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Auto_AudioProcessor::acceptsMidi() const
{
    return false;
}

bool Auto_AudioProcessor::producesMidi() const
{
    return false;
}

bool Auto_AudioProcessor::isMidiEffect() const
{
    return false;
}

double Auto_AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Auto_AudioProcessor::getNumPrograms()
{
    return 1;  
}

int Auto_AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Auto_AudioProcessor::setCurrentProgram (int index)
{
}

const String Auto_AudioProcessor::getProgramName (int index)
{
    return {};
}

void Auto_AudioProcessor::changeProgramName (int index, const String& newName)
{
}



void Auto_AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = dsp::ProcessSpec {sampleRate,
        static_cast<uint32>(samplesPerBlock),
        static_cast<uint32>(getTotalNumInputChannels())};


    // Avoid clicks during gain parameter change
    chain.get<outputGainIndex>().setRampDurationSeconds(0.1);
    chain.get<inputGainIndex>().setRampDurationSeconds(0.1);
    chain.get<filterIndex>().setMode(LadderFilterMode::LPF12);
    chain.get<filterIndex>().setEnvFollowerPtr(&chain.get<followerIndex>());

    // Set source for dry buffer of mix control
    chain.get<mixerIndex>().setOtherBlock(chain.get<bufferStoreIndex>().getAudioBlockPointer());

    updateAllParameters();
    chain.prepare(spec);
}

void Auto_AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto block = dsp::AudioBlock<float>(buffer);
    const auto context = dsp::ProcessContextReplacing<float>(block);
    updateAllParameters();
    chain.process(context);
}

void Auto_AudioProcessor::releaseResources()
{
    chain.get<mixerIndex>().setOtherBlock(nullptr);
    chain.get<followerIndex>().onValueCalculated = nullptr;
    chain.reset();
}

EnvelopeFollower* Auto_AudioProcessor::getEnvelopeFollower() 
{
    return &chain.get<followerIndex>();
}

FilterFollower<float>* Auto_AudioProcessor::getLadderFilter() 
{
    return &chain.get<filterIndex>();
}

Meter* Auto_AudioProcessor::getInputMeter() 
{
    return &chain.get<inputMeterIndex>();
}

Meter* Auto_AudioProcessor::getOutputMeter() 
{
    return &chain.get<outputMeterIndex>();
}

Graph* Auto_AudioProcessor::getGraph()
{
    return &chain.get<graphIndex>();
}

AudioProcessorValueTreeState::ParameterLayout Auto_AudioProcessor::getParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::INPUT_GAIN_ID, "Input Gain",   NormalisableRange(-24.f, 24.f, 0.1f), 0.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::OUTPUT_GAIN_ID, "Output Gain", NormalisableRange(-24.f, 24.f, 0.1f), 0.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::DRIVE_ID, "Drive",            NormalisableRange(1.f, 10.f, 0.1f), 1.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::RESONANCE_ID, "Resonance",    NormalisableRange(0.f, 1.f, 0.01f), 0.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::FREQUENCY_ID, "Frequency",    NormalisableRange(20.0f, 20000.0f, 1.f, 0.5f), 1000.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::MIX_ID, "Mix",                NormalisableRange(0.f, 100.f, 1.f), 100.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_AMOUNT_ID, "Env %",        NormalisableRange(0.f, 5.f, 0.01f), 1.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_ATTACK_ID, "Attack Speed",        NormalisableRange(0.f, 1000.f, 1.f), 100.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_RELEASE_ID, "Release Speed",        NormalisableRange(0.f, 1000.f, 1.f), 100.f));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::ENV_ADVANCED_ID, "Advanced", false));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::ENV_SPEED_ID, "Env Speed", false));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::TWO_FOUR_POLE_ID, "12/24", false));

    return layout;
}

void Auto_AudioProcessor::updateAllParameters()
{
    chain.get<inputGainIndex>().setGainDecibels(apvts.getRawParameterValue(parameter_constants::INPUT_GAIN_ID)->load());
    chain.get<outputGainIndex>().setGainDecibels(apvts.getRawParameterValue(parameter_constants::OUTPUT_GAIN_ID)->load());
    chain.get<filterIndex>().setCutoffFrequencyHz(apvts.getRawParameterValue(parameter_constants::FREQUENCY_ID)->load());
    chain.get<filterIndex>().setResonance(apvts.getRawParameterValue(parameter_constants::RESONANCE_ID)->load());
    chain.get<filterIndex>().setDrive(apvts.getRawParameterValue(parameter_constants::DRIVE_ID)->load());
    auto& filterFollower = chain.get<followerIndex>();
    if (apvts.getRawParameterValue(parameter_constants::ENV_ADVANCED_ID)->load())
    {
        filterFollower.setAttack(apvts.getRawParameterValue(parameter_constants::ENV_ATTACK_ID)->load());
        filterFollower.setRelease(apvts.getRawParameterValue(parameter_constants::ENV_RELEASE_ID)->load());
    }
    else
    {
        auto speed = apvts.getRawParameterValue(parameter_constants::ENV_SPEED_ID)->load() ? filterFollower.getSlowTime() : filterFollower.getFastTime();
        filterFollower.setAttack(speed);
        filterFollower.setRelease(speed);
    }
    if(apvts.getRawParameterValue(parameter_constants::TWO_FOUR_POLE_ID)->load())
        chain.get<filterIndex>().setMode(LadderFilterMode::LPF12);
    else
        chain.get<filterIndex>().setMode(LadderFilterMode::LPF24);

    chain.get<followerIndex>().setAmount(apvts.getRawParameterValue(parameter_constants::ENV_AMOUNT_ID)->load()); 
    chain.get<mixerIndex>().setMix(apvts.getRawParameterValue(parameter_constants::MIX_ID)->load());
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Auto_AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif


bool Auto_AudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Auto_AudioProcessor::createEditor()
{
    return new Auto_AudioProcessorEditor (*this);
}

void Auto_AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void Auto_AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Auto_AudioProcessor();
}
