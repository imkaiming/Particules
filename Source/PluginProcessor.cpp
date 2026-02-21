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
	paramsView(), grainEngine(paramsView)
#endif
{
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
	int numChannels = getTotalNumOutputChannels();
	paramsView.init(apvts, sampleRate);
	grainEngine.init(static_cast<int>(sampleRate), numChannels, samplesPerBlock);

	//juce::Logger::outputDebugString("density : " + (const juce::String)this->paramsView.getDensity() + " duration : " + (const juce::String)this->paramsView.getDuration());
#if ENABLE_DEBUG_PRESET // exist also in the audio file loader
	loadDebugPreset();
#endif
}

void ParticulesAudioProcessor::releaseResources()
{
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

	if(paramsView.getSampleSource() && paramsView.getIsPlaying() || !paramsView.getIsGrainsEmpty())
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
	ParticulesAudioProcessorEditor* editor = new ParticulesAudioProcessorEditor(*this);
	return editor;
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
	}

	//juce::Logger::outputDebugString("apvts mix : " + (juce::String)apvts.getRawParameterValue(MIX_ID)->load());
	//juce::Logger::outputDebugString("apvts gain : " + (juce::String)apvts.getRawParameterValue(GAIN_ID)->load());
	//juce::Logger::outputDebugString("apvts dens : " + (juce::String)apvts.getRawParameterValue(DENSITY_ID)->load());
	//juce::Logger::outputDebugString("apvts dur : " + (juce::String)apvts.getRawParameterValue(DURATION_ID)->load());
	//juce::Logger::outputDebugString("apvts pitch : " + (juce::String)apvts.getRawParameterValue(PITCH_ID)->load());

}

juce::AudioProcessorValueTreeState::ParameterLayout ParticulesAudioProcessor::createParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Mix::id, Param::Mix::name, juce::NormalisableRange<float>(Param::Mix::min, Param::Mix::max, 0.01f),
			Param::Mix::init, " %", juce::AudioProcessorParameter::genericParameter,
			[](float v, int) { return juce::String(v, 1) + "%"; }, [](const juce::String& s) { return s.getFloatValue(); })
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::Gain::id, Param::Gain::name, juce::NormalisableRange<float>(Param::Gain::min, Param::Gain::max, 0.01f),
			Param::Gain::init, " dB", juce::AudioProcessorParameter::genericParameter,
			[](float v, int) { return juce::String(v, 3) + " dB"; }, [](const juce::String& s) { return s.getFloatValue(); })
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

	const juce::StringArray choicesEnvTypeNames(Param::EnvelopeType::envTypeNames.data(), Param::EnvelopeType::envTypeNames.size());
	//const juce::StringArray choices{Param::EnvelopeType::envTypeNames, sizeof(Param::EnvelopeType::envTypeNames)};

	layout.add(
		std::make_unique<juce::AudioParameterChoice>(
			Param::EnvelopeType::id, Param::EnvelopeType::name, choicesEnvTypeNames, 0)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			Param::EnvelopeWidth::id,
			Param::EnvelopeWidth::name,
			juce::NormalisableRange<float>(Param::EnvelopeWidth::min, Param::EnvelopeWidth::max, 0.01f),
			Param::EnvelopeWidth::init)
	);

	//const juce::StringArray choicesTraversalModeNames(Param::TraversalMode::traversalModeNames.data(), Param::TraversalMode::traversalModeNames.size());

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

void ParticulesAudioProcessor::loadDebugPreset()
{
	if(debugPresetLoaded) return;
	apvts.getParameter(Param::Mix::id)->setValueNotifyingHost(1.f);// MIX100%
	apvts.getParameter(Param::Gain::id)->setValueNotifyingHost(juce::Decibels::decibelsToGain(-3.f));
	apvts.getParameter(Param::Density::id)->setValueNotifyingHost(1.f);
	apvts.getParameter(Param::Duration::id)->setValueNotifyingHost(1.f);
	apvts.getParameter(Param::Speed::id)->setValueNotifyingHost(1.f);
	apvts.getParameter(Param::Position::id)->setValueNotifyingHost(0.5f);
	apvts.getParameter(Param::Selection::id)->setValueNotifyingHost(0.25f);
	apvts.getParameter(Param::EnvelopeType::id)->setValueNotifyingHost(1.f);

	//apvts.getParameter(Param::EnvelopeWidth::id)->setValueNotifyingHost(100.f);
	//apvts.getParameter(Param::TraversalMode::id)->;
	//apvts.getParameter(Param::TraversalTime::id)->;

	// charger le fichier dans le audio file loader


	debugPresetLoaded = true;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ParticulesAudioProcessor();
}
