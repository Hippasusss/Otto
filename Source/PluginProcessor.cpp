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
    chain.get<filterIndex>().setMode(dsp::LadderFilterMode::LPF12);

    // Set source for dry buffer of mix control
    chain.get<mixerIndex>().setOtherBlock(chain.get<bufferStoreIndex>().getAudioBlockPointer());

    // Register envelope follower callback to set frequency parameter at runtime
    dsp::LadderFilter<float>& filter = chain.get<filterIndex>();
    chain.get<followerIndex>().onValueCalculated = [&](const float value) 
    {
        // Highest user settable frequency
        const float maxFrequency = apvts.getParameterRange("frequency").end;
        // current user set frequency
        const float frequencySet = apvts.getRawParameterValue("frequency")->load();
        // frequencies above the user set frequency
        const float frequencyRemainder = maxFrequency - frequencySet;

        const auto modulatedFrequency = jlimit<float>(20, 20000, (frequencySet + value * frequencyRemainder));
        filter.setCutoffFrequencyHz(modulatedFrequency);
    };

    // initalise
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

    // Split the audio block into multiple sub buffers. Seems like a daft/naive way of doing this.
    // need to find a proper way to be able to set the cutoff frequency at smaller increments of the buffer size.
    // without this a buffer size of 2048 would limit the frequency to be set every 42ms (at 48k). that's shite.
    // must be a smarty pants way to do this. 
    //const int subBlockSize = 32;
    //const int numSubBlocks = buffer.getNumSamples() / 32;
    //for(int i = 0; i < numSubBlocks; ++i)
    //{
    //    dsp::AudioBlock<float> subBlock = block.getSubBlock(subBlockSize * i, subBlockSize);
    //    const auto context = dsp::ProcessContextReplacing<float>(subBlock);
    //    chain.process(context);
    //}

}

void Auto_AudioProcessor::releaseResources()
{
    chain.get<mixerIndex>().setOtherBlock(nullptr);
    chain.get<followerIndex>().onValueCalculated = nullptr;
    chain.reset();
}

const EnvelopeFollower& Auto_AudioProcessor::getEnvelopeFollower() const
{
    return chain.get<followerIndex>();
}

const dsp::LadderFilter<float>& Auto_AudioProcessor::getLadderFilter() const
{
    return chain.get<filterIndex>();
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
    layout.add(std::make_unique<AudioParameterFloat>(parameter_constants::ENV_AMOUNT_ID, "Env %",        NormalisableRange(0.f, 100.f, 1.f), 100.f));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::ENV_SPEED_ID, "Env Speed", false));
    layout.add(std::make_unique<AudioParameterBool>(parameter_constants::TWO_FOUR_POLE_ID, "12/24", false));

    return layout;
}

float Auto_AudioProcessor::getAPVTSValue(const String& ID)
{
    return apvts.getRawParameterValue(ID)->load();
}

void Auto_AudioProcessor::updateAllParameters()
{
    chain.get<inputGainIndex>().setGainDecibels(getAPVTSValue(parameter_constants::INPUT_GAIN_ID));
    chain.get<outputGainIndex>().setGainDecibels(getAPVTSValue(parameter_constants::OUTPUT_GAIN_ID));
    chain.get<filterIndex>().setCutoffFrequencyHz(getAPVTSValue(parameter_constants::FREQUENCY_ID));
    chain.get<filterIndex>().setResonance(getAPVTSValue(parameter_constants::RESONANCE_ID));
    chain.get<filterIndex>().setDrive(getAPVTSValue(parameter_constants::DRIVE_ID));
    chain.get<followerIndex>().setAmount(getAPVTSValue(parameter_constants::ENV_AMOUNT_ID));
    chain.get<mixerIndex>().setMix(getAPVTSValue(parameter_constants::MIX_ID));
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
