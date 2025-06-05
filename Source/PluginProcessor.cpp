#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Constants.h"

Auto_AudioProcessor::Auto_AudioProcessor()
{
    chain.get<filterIndex>().setEnvFollowerPtr(&chain.get<followerIndex>());
}
Auto_AudioProcessor::~Auto_AudioProcessor() = default;

const String Auto_AudioProcessor::getName() const
{
    return "Otto";
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
    for (auto& overSampler: oversamplers) {
        overSampler.numChannels = getTotalNumInputChannels();
        overSampler.initProcessing(static_cast<size_t>(samplesPerBlock));
        overSampler.setUsingIntegerLatency(false);
        overSampler.reset();
    }

    auto spec = dsp::ProcessSpec {sampleRate,
        static_cast<uint32>(samplesPerBlock),
        static_cast<uint32>(getTotalNumInputChannels())};

    auto overSpec = dsp::ProcessSpec {sampleRate * currentOversampler->getOversamplingFactor(),
        static_cast<uint32>(samplesPerBlock * currentOversampler->getOversamplingFactor()),
        static_cast<uint32>(getTotalNumInputChannels())};


    outputChain.get<outputGainIndex>().setRampDurationSeconds(0.1);
    inputChain.get<inputGainIndex>().setRampDurationSeconds(0.1);
    chain.get<filterIndex>().setMode(LadderFilterMode::LPF12);
    chain.get<filterIndex>().setEnvFollowerPtr(&chain.get<followerIndex>());

    outputChain.get<mixerIndex>().setOtherBlock(inputChain.get<bufferStoreIndex>().getAudioBlockPointer());

    updateAllParameters();
    inputChain.prepare(spec);
    chain.prepare(overSpec);
    outputChain.prepare(spec);
}
void Auto_AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    updateAllParameters();
    ScopedNoDenormals noDenormals;

    auto inputBlock = dsp::AudioBlock<float>(buffer);
    const auto context = dsp::ProcessContextReplacing<float>(inputBlock);

    inputChain.process(context);

    auto oversampled = currentOversampler->processSamplesUp(inputBlock);
    const auto overContext = dsp::ProcessContextReplacing<float>(oversampled);
    chain.process(overContext);
    currentOversampler->processSamplesDown(inputBlock);

    outputChain.process(context);
}

void Auto_AudioProcessor::releaseResources()
{
    outputChain.get<mixerIndex>().setOtherBlock(nullptr);
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
    return &inputChain.get<inputMeterIndex>();
}

Meter* Auto_AudioProcessor::getOutputMeter() 
{
    return &outputChain.get<outputMeterIndex>();
}

Graph* Auto_AudioProcessor::getGraph()
{
    return &inputChain.get<graphIndex>();
}

dsp::Oversampling<float>* Auto_AudioProcessor::getOversampling()
{
    return currentOversampler;
}

void Auto_AudioProcessor::changeOversampling(size_t factor)
{
    suspendProcessing(true);
    currentOversampler = &oversamplers[factor];
    //TODO: fix this, you're not meant to do it.
    const auto sampleRate = getSampleRate();
    const auto blockSize = getBlockSize();
    prepareToPlay(sampleRate, blockSize);
    setLatencySamples(currentOversampler->getLatencyInSamples());
    suspendProcessing(false);
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
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_AMOUNT_ID, "Env %",        NormalisableRange(0.f, 10.f, 0.01f), 1.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_ATTACK_ID, "Attack Speed",        NormalisableRange(0.f, 1000.f, 1.f), 100.f));
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_RELEASE_ID, "Release Speed",        NormalisableRange(0.f, 1000.f, 1.f), 100.f));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::ENV_ADVANCED_ID, "Advanced", false));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::ENV_SPEED_ID, "Env Speed", false));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::TWO_FOUR_POLE_ID, "12/24", false));

    return layout;
}

void Auto_AudioProcessor::updateAllParameters()
{
    inputChain.get<inputGainIndex>().setGainDecibels(apvts.getRawParameterValue(parameter_constants::INPUT_GAIN_ID)->load());
    outputChain.get<outputGainIndex>().setGainDecibels(apvts.getRawParameterValue(parameter_constants::OUTPUT_GAIN_ID)->load());
    chain.get<filterIndex>().setCutoffFrequencyHz(apvts.getRawParameterValue(parameter_constants::FREQUENCY_ID)->load());
    chain.get<filterIndex>().setResonance(apvts.getRawParameterValue(parameter_constants::RESONANCE_ID)->load());
    chain.get<filterIndex>().setDrive(apvts.getRawParameterValue(parameter_constants::DRIVE_ID)->load());
    auto& envelopeFollower = chain.get<followerIndex>();
    if (apvts.getRawParameterValue(parameter_constants::ENV_ADVANCED_ID)->load())
    {
        envelopeFollower.setAttack(apvts.getRawParameterValue(parameter_constants::ENV_ATTACK_ID)->load());
        envelopeFollower.setRelease(apvts.getRawParameterValue(parameter_constants::ENV_RELEASE_ID)->load());
    }
    else
    {
        auto speed = apvts.getRawParameterValue(parameter_constants::ENV_SPEED_ID)->load() ? envelopeFollower.getSlowTime() : envelopeFollower.getFastTime();
        envelopeFollower.setAttack(speed);
        envelopeFollower.setRelease(speed);
    }
    if(apvts.getRawParameterValue(parameter_constants::TWO_FOUR_POLE_ID)->load())
        chain.get<filterIndex>().setMode(LadderFilterMode::LPF12);
    else
        chain.get<filterIndex>().setMode(LadderFilterMode::LPF24);

    chain.get<followerIndex>().setAmount(apvts.getRawParameterValue(parameter_constants::ENV_AMOUNT_ID)->load()); 
    outputChain.get<mixerIndex>().setMix(apvts.getRawParameterValue(parameter_constants::MIX_ID)->load());
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Auto_AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    return true;
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
