#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "parameter_constants.h"

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
inputGain(parameter_constants::INPUT_GAIN_ID, "In", NormalisableRange<float>(-30,30), 0),
outputGain(parameter_constants::OUTPUT_GAIN_ID, "Out", NormalisableRange<float>(-30,30), 0),
resonance(parameter_constants::RESONANCE_ID, "Res", NormalisableRange<float>(0,1), 0),
frequency(parameter_constants::FREQUENCY_ID, "Freq", NormalisableRange<float>(20, 20000, 0.1f), 0.1),
drive(parameter_constants::DRIVE_ID, "Drive", NormalisableRange<float>(1,10), 1),
envAmount(parameter_constants::ENV_AMOUNT_ID, "Env Am", NormalisableRange<float>(0,1), 0),
mix(parameter_constants::MIX_ID, "Mix", NormalisableRange<float>(0,1), 0),
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

	chain.get<outputGainIndex>().setRampDurationSeconds(0.1);
	chain.get<inputGainIndex>().setRampDurationSeconds(0.1);
    initaliseParameters();
    chain.prepare(spec);

    dsp::LadderFilter<float>& filter = chain.get<filterIndex>();
	chain.get<followerIndex>().callback = [this, &filter ](const float value)
	{
        const float modulatedFrequency = jlimit<float>(0.1, 20000, this->frequency.get() + value * 20000);
        filter.setCutoffFrequencyHz(modulatedFrequency);
	};
}

void Auto_AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto block = dsp::AudioBlock<float>(buffer);
    const auto context = dsp::ProcessContextReplacing<float>(block);

    chain.process(context);
}
void Auto_AudioProcessor::releaseResources()
{
    chain.reset();
}

void Auto_AudioProcessor::sliderValueChanged(Slider* slider)
{
	const String& ID = slider->getComponentID();
	const float value = static_cast<float>(slider->getValue());
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

void Auto_AudioProcessor::setParameter(const String& parameterID) 
{
    if(parameterID == parameter_constants::INPUT_GAIN_ID)
    {
		chain.get<inputGainIndex>().setGainDecibels(inputGain);
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
    else if(parameterID == parameter_constants::ENV_SPEED_ID)
    {
		const auto rate = envSpeed ? fast : slow;
		chain.get<followerIndex>().setAttack(rate);
		chain.get<followerIndex>().setRelease(rate);
    }
    else if(parameterID == parameter_constants::ENV_AMOUNT_ID)
    {
		chain.get<followerIndex>().setAmount(envAmount);
    }
    else if(parameterID == parameter_constants::TWO_FOUR_POLE_ID)
    {
		const auto mode = twoFourPole ? dsp::LadderFilter<float>::Mode::LPF24 :
										dsp::LadderFilter<float>::Mode::LPF12;
		chain.get<filterIndex>().setMode(mode);
    }
    else if(parameterID == parameter_constants::OUTPUT_GAIN_ID)
    {
		chain.get<outputGainIndex>().setGainDecibels(outputGain);
    }
}

void Auto_AudioProcessor::initaliseParameters()
{
	chain.get<inputGainIndex>().setGainDecibels(inputGain);
	chain.get<filterIndex>().setDrive(drive);
	chain.get<filterIndex>().setCutoffFrequencyHz(frequency);
	chain.get<filterIndex>().setResonance(resonance);
	const auto rate = envSpeed ? fast : slow;
	chain.get<followerIndex>().setAttack(rate);
	chain.get<followerIndex>().setRelease(rate);
	chain.get<followerIndex>().setAmount(envAmount);
	const auto mode = twoFourPole ? dsp::LadderFilter<float>::Mode::LPF24 :
									dsp::LadderFilter<float>::Mode::LPF12;
	chain.get<filterIndex>().setMode(mode);
	chain.get<outputGainIndex>().setGainDecibels(outputGain);
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool Auto_AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


//==============================================================================
bool Auto_AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Auto_AudioProcessor::createEditor()
{
    return new Auto_AudioProcessorEditor (*this);
}

//==============================================================================
void Auto_AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Auto_AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Auto_AudioProcessor();
}
