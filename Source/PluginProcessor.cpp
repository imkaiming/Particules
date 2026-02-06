/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
ParticulesAudioProcessor::ParticulesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
					 .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
					 .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", createParameterLayout()), //apvts stands for audio processor value tree state
	grainEngine(), stateParams()
#endif
{
	// la connection entre le ValueTreeState et le StateParameters ce fait ici
	//apvts.addParameterListener(MIX_ID, &stateParams);
	//apvts.addParameterListener(GAIN_ID, &stateParams);
	//apvts.addParameterListener(DENSITY_ID, &stateParams);
	//apvts.addParameterListener(DURATION_ID, &stateParams);
	//apvts.addParameterListener(PITCH_ID, &stateParams);

	// add the pan

}

ParticulesAudioProcessor::~ParticulesAudioProcessor()
{
}

const juce::String ParticulesAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool ParticulesAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ParticulesAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ParticulesAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double ParticulesAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ParticulesAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int ParticulesAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ParticulesAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ParticulesAudioProcessor::getProgramName(int index)
{
	return {};
}

void ParticulesAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void ParticulesAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	stateParams.init(apvts, getTotalNumInputChannels(), sampleRate);

	//stateParams.setSampleRate();
	//this->stateParams.getNumChannels(getTotalNumInputChannels()) // préférer ça que passer la variable
	grainEngine.init(&stateParams, static_cast<int>(sampleRate), getTotalNumOutputChannels(), samplesPerBlock);

	//adsr.setSampleRate(sampleRate);
	//adsr.setParameters(adsrParameters);



	//juce::Logger::outputDebugString("density : " + (const juce::String)this->stateParams.getDensity()
	//	+ " duration : " + (const juce::String)this->stateParams.getDuration());

}

void ParticulesAudioProcessor::releaseResources()
{
	//grainEngine.initialization();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParticulesAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if(layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
	   && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if(layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void ParticulesAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples();

	for(int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, numSamples);

	//adsrParameters.attack = stateParams->getAttack();
	//adsrParameters.decay = stateParams->getDecay();
	//adsrParameters.sustain = stateParams->getSustain();
	//adsrParameters.release = stateParams->getRelease();

	//for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
	//	juce::ADSR::applyEnvelopeToBuffer(buffer, channel, numSamples);
	//}

	// let the engine operate under the given buffer if the button is press
	if(stateParams.getAudioLoaded() && stateParams.getIsPlaying() || !stateParams.getIsGrainsEmpty())
	{
		grainEngine.process(buffer, numSamples);
	}
}

bool ParticulesAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ParticulesAudioProcessor::createEditor()
{
	return new ParticulesAudioProcessorEditor(*this);
	//return new juce::GenericAudioProcessorEditor(*this);
}

void ParticulesAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	juce::ValueTree vt = apvts.copyState();
	std::unique_ptr<juce::XmlElement> xml(vt.createXml());
	copyXmlToBinary(*xml, destData);
}

void ParticulesAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if(xmlState.get() != nullptr)
	{
		// si on trouve un fichier xml alors on met à jours les données dans apvts
		if(xmlState->hasTagName(apvts.state.getType()))
			apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
	} else
	{
		// on initialise tout avec les valeurs de base
		//initValueTreeState();
	}

	//juce::Logger::outputDebugString("apvts mix : " + (juce::String)apvts.getRawParameterValue(MIX_ID)->load());
	//juce::Logger::outputDebugString("apvts gain : " + (juce::String)apvts.getRawParameterValue(GAIN_ID)->load());
	//juce::Logger::outputDebugString("apvts dens : " + (juce::String)apvts.getRawParameterValue(DENSITY_ID)->load());
	//juce::Logger::outputDebugString("apvts dur : " + (juce::String)apvts.getRawParameterValue(DURATION_ID)->load());
	//juce::Logger::outputDebugString("apvts pitch : " + (juce::String)apvts.getRawParameterValue(PITCH_ID)->load());

	//stateParams.print();
}

//void ParticulesAudioProcessor::initValueTreeState()
//{
//	apvts.getRawParameterValue(Param::Mix::id)->store(Param::Mix::init);
//	apvts.getRawParameterValue(Param::Gain::id)->store(Param::Gain::init);
//	apvts.getRawParameterValue(Param::Density::id)->store(Param::Density::init);
//	apvts.getRawParameterValue(Param::Duration::id)->store(Param::Duration::init);
//	apvts.getRawParameterValue(Param::Speed::id)->store(Param::Speed::init);
//	apvts.getRawParameterValue(Param::EnvelopeType::id)->store(Param::EnvelopeType::init);
//	apvts.getRawParameterValue(Param::Position::id)->store(Param::Position::init);
//	apvts.getRawParameterValue(Param::Selection::id)->store(Param::Selection::init);
//	apvts.getRawParameterValue(Param::EnvelopeWidth::id)->store(Param::EnvelopeWidth::init);
//	apvts.getRawParameterValue(Param::TraversalMode::id)->store(Param::TraversalMode::init);
//	apvts.getRawParameterValue(Param::TraversalTime::id)->store(Param::TraversalTime::init);
//}

StateParameters* ParticulesAudioProcessor::getStateParameters()
{
	return &stateParams;
}

ValueTreeState* ParticulesAudioProcessor::getValueTreeState()
{
	return &apvts;
}

juce::AudioProcessorValueTreeState::ParameterLayout ParticulesAudioProcessor::createParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(
		std::make_unique<juce::AudioParameterInt>(
			Param::Mix::id,
			Param::Mix::name,
			Param::Mix::min, Param::Mix::max, Param::Mix::init)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Gain::id,
			Param::Gain::name,
			juce::NormalisableRange<float>(Param::Gain::min, Param::Gain::max, 0.01f),
			Param::Gain::init)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			// id, name, min, max, init val
			Param::Density::id,
			Param::Density::name,
			juce::NormalisableRange<float>(Param::Density::min, Param::Density::max, 0.001f),
			Param::Density::init)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Duration::id,
			Param::Duration::name,
			juce::NormalisableRange<float>(Param::Duration::min, Param::Duration::max, 0.001f),
			Param::Duration::init)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Speed::id,
			Param::Speed::name,
			juce::NormalisableRange<float>(Param::Speed::min, Param::Speed::max, 0.001f),
			Param::Speed::init)
	);

	//layout.add(
	//	std::make_unique<juce::AudioParameterFloat>(
	//		PITCH_ID,
	//		PITCH_NAME,
	//		PITCH_MIN, PITCH_MAX, PITCH_DEFAULT)
	//);

	//layout.add(
	//	std::make_unique<juce::String>(
	//		FILEPATH_ID,
	//		FILEPATH_NAME,
	//		FILEPATH_DEFAULT)
	//);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Position::id,
			Param::Position::name,
			juce::NormalisableRange<float>(Param::Position::min, Param::Position::max, 0.001f),
			Param::Position::init)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Selection::id,
			Param::Selection::name,
			juce::NormalisableRange<float>(Param::Selection::min, Param::Selection::max, 0.001f),
			Param::Selection::init)
	);

	const juce::StringArray& choices{Param::EnvelopeType::ENVTYPE_1,
		Param::EnvelopeType::ENVTYPE_2,
		Param::EnvelopeType::ENVTYPE_3,
		Param::EnvelopeType::ENVTYPE_4,
		Param::EnvelopeType::ENVTYPE_5,
		Param::EnvelopeType::ENVTYPE_6,
		Param::EnvelopeType::ENVTYPE_7};

	layout.add(
		std::make_unique<juce::AudioParameterInt>(
			Param::EnvelopeType::id, Param::EnvelopeType::name, 1, 7, Param::EnvelopeType::init)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::EnvelopeWidth::id,
			Param::EnvelopeWidth::name,
			juce::NormalisableRange<float>(Param::EnvelopeWidth::min, Param::EnvelopeWidth::max, 0.01f),
			Param::EnvelopeWidth::init)
	);


	layout.add(
		std::make_unique<juce::AudioParameterInt>(
			Param::TraversalMode::id, Param::TraversalMode::name, 1, 5, Param::TraversalMode::init)
	);


	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::TraversalTime::id,
			Param::TraversalTime::name,
			juce::NormalisableRange<float>(Param::TraversalTime::min, Param::TraversalTime::max, 0.001f),
			Param::TraversalTime::init)
	);

	// ajouter pan, direction 
	// randomDensity, randomDuration, randomPan, randomDirection, randomPitch

	return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ParticulesAudioProcessor();
}
