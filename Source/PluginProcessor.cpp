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
	  ),
	inputGain(parameter_constants::INPUT_GAIN_ID, "In", -30, 30, 0),
	outputGain(parameter_constants::OUTPUT_GAIN_ID, "Out", -30,30, 0),
	resonance(parameter_constants::RESONANCE_ID, "Res", 0, 1, 0),
	frequency(parameter_constants::FREQUENCY_ID, "Freq", 20, 20000, 0.1),
	drive(parameter_constants::DRIVE_ID, "Drive", 1,10, 1),
	envAmount(parameter_constants::ENV_AMOUNT_ID, "Env Am", 0, 100, 0),
	mix(parameter_constants::MIX_ID, "Mix", 0, 100, 0),
	envSpeed(parameter_constants::ENV_SPEED_ID, "Env Speed", false),
	twoFourPole(parameter_constants::TWO_FOUR_POLE_ID, "2/4 Pole", false)
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
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Auto_AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Auto_AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
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

    // Set source for dry buffer of mix control
	chain.get<mixerIndex>().setOtherBlock(chain.get<bufferStoreIndex>().getAudioBlockPointer());

    // Register envelope follower callback to set frequency parameter at runtime
    dsp::LadderFilter<float>& filter = chain.get<filterIndex>();
	chain.get<followerIndex>().onValueCalculated = [&](const float value) 
	{
        const float maxFrequency = frequency.range.end;
        const float frequencySet = frequency;
        const float frequencyRemainder = maxFrequency - frequencySet;
        const auto modulatedFrequency = jlimit<float>(20, 20000, (this->frequency + value * frequencyRemainder));
        filter.setCutoffFrequencyHz(modulatedFrequency);
	};

    // initalise
    chain.prepare(spec);
    initaliseParameters();
}

//Todo: split process block into parts so as processing is the same regardless of block size
void Auto_AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto block = dsp::AudioBlock<float>(buffer);
    const auto context = dsp::ProcessContextReplacing<float>(block);
    chain.process(context);
}

void Auto_AudioProcessor::releaseResources()
{
	chain.get<mixerIndex>().setOtherBlock(nullptr);
	chain.get<followerIndex>().onValueCalculated = nullptr;
    chain.reset();
}

void Auto_AudioProcessor::sliderValueChanged(Slider* slider)
{
	const String& ID = slider->getComponentID();
	const auto value = static_cast<float>(slider->getValue());

    for(auto floatParam : floatParameter)
    {
        if(floatParam->paramID == ID)
        {
	       *floatParam = value; 
           setParameter(ID);
           break;
        }
    }
}

void Auto_AudioProcessor::buttonClicked(Button* button)
{
    const String& ID = button->getComponentID();
    const bool value = button->getToggleState();
	
    for(auto boolParam : boolParameter)
    {
        if(boolParam->paramID == ID)
        {
	       *boolParam = value;
           setParameter(ID);
           break;
        }
    }
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


void Auto_AudioProcessor::setParameter(const String& parameterID) 
{
    // Input Gain
    if(parameterID == parameter_constants::INPUT_GAIN_ID)
    {
		chain.get<inputGainIndex>().setGainDecibels(inputGain);
    }

	// Filter
    else if(parameterID == parameter_constants::TWO_FOUR_POLE_ID)
    {
		const auto mode = twoFourPole ? dsp::LadderFilter<float>::Mode::LPF24 :
										dsp::LadderFilter<float>::Mode::LPF12;
		chain.get<filterIndex>().setMode(mode);
    }
    else if(parameterID == parameter_constants::DRIVE_ID)
    {
		chain.get<filterIndex>().setDrive(drive);
    }
    else if(parameterID ==  parameter_constants::FREQUENCY_ID)
    {
		chain.get<filterIndex>().setCutoffFrequencyHz(frequency);
    }
    else if(parameterID == parameter_constants::RESONANCE_ID)
    {
		chain.get<filterIndex>().setResonance(resonance);
    }

    // Follower
    else if(parameterID == parameter_constants::ENV_SPEED_ID)
    {
		const auto rate = envSpeed ? fast : slow;
		chain.get<followerIndex>().setAttack(rate);
		chain.get<followerIndex>().setRelease(rate);
    }
    else if(parameterID == parameter_constants::ENV_AMOUNT_ID)
    {
		chain.get<followerIndex>().setAmount(envAmount.getNormalised());
    }

    // Mixer
    else if(parameterID == parameter_constants::MIX_ID)
    {
		chain.get<mixerIndex>().setMix(mix.getNormalised());
    }

	// Output Gain
    else if(parameterID == parameter_constants::OUTPUT_GAIN_ID)
    {
		chain.get<outputGainIndex>().setGainDecibels(outputGain);
    }
}

void Auto_AudioProcessor::initaliseParameters()
{
	for(auto& floatParam : floatParameter)
	{
		setParameter(floatParam->paramID);
	}
	for(auto& boolParam : boolParameter)
	{
		setParameter(boolParam->paramID);
	}
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
}

void Auto_AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Auto_AudioProcessor();
}
