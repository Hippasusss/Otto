#include "PluginProcessor.h"
#include "PluginEditor.h"

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
inputGain("inputGain", "In", NormalisableRange<float>(0,1), 0),
outputGain("outputGain", "Out", NormalisableRange<float>(0,1), 0),
resonance("resonance", "Res", NormalisableRange<float>(0,2), 0),
frequency("frequency", "Freq", NormalisableRange<float>(0.1,20000), 0.1),
drive("drive", "Drive", NormalisableRange<float>(1,10), 1),
envAmount("envAmount", "Env Am", NormalisableRange<float>(0,1), 0),
mix("mix", "Mix", NormalisableRange<float>(0,1), 0),
envSpeed("envSpeed", "Env Speed", false),
twoFourPole("twoFourPole", "2/4 Pole", false)
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
	const auto spec = dsp::ProcessSpec {sampleRate, static_cast<uint32>(samplesPerBlock),static_cast<uint32>(getTotalNumInputChannels())};
    chain.prepare(spec);
}

void Auto_AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto block = dsp::AudioBlock<float>(buffer);
    const auto context = dsp::ProcessContextReplacing<float>(block);
    setParameters();
    chain.process(context);
}
void Auto_AudioProcessor::releaseResources()
{
    chain.reset();
}

void Auto_AudioProcessor::sliderValueChanged(Slider* slider)
{
    auto ID = slider->getComponentID();
    auto value = slider->getValue();
    for(auto floatParam : floatParameter)
    {
        if(floatParam->paramID == ID)
        {
	       *floatParam = static_cast<float>(value); 
        }
    }
}

void Auto_AudioProcessor::buttonClicked(Button* button)
{

    for(auto boolParam : boolParameter)
    {
        if(boolParam->paramID == button->getComponentID())
        {
	       *boolParam = button->getToggleState(); 
        }
    }
}

void Auto_AudioProcessor::setParameters()
{

    chain.get<inputGainIndex>().setGainLinear(inputGain);

    const auto rate = envSpeed ? 0.2 : 0.7;
    chain.get<followerIndex>().setAttack(rate);
    chain.get<followerIndex>().setRelease(rate);

	const auto mode = twoFourPole ? dsp::LadderFilter<float>::Mode::LPF24 :
									dsp::LadderFilter<float>::Mode::LPF12;
	const auto freq = jlimit<float>(0.1f, 20000, frequency + chain.get<followerIndex>().getValue());
    chain.get<filterIndex>().setCutoffFrequencyHz(freq);
    chain.get<filterIndex>().setResonance(resonance);
    chain.get<filterIndex>().setDrive(drive);
    chain.get<filterIndex>().setMode(mode);

    chain.get<outputGainIndex>().setGainLinear(outputGain);
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
