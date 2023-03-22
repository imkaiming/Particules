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
	), apvts(*this, nullptr, "Parameters", createParameterLayout()),
	grainEngine(&stateParams)
#endif
{
	// la connection entre le ValueTreeState et le StateParameters ce fait ici
	//apvts.addParameterListener(MIX_ID, &stateParams);
	//apvts.addParameterListener(GAIN_ID, &stateParams);
	//apvts.addParameterListener(DENSITY_ID, &stateParams);
	//apvts.addParameterListener(DURATION_ID, &stateParams);
	//apvts.addParameterListener(PITCH_ID, &stateParams);

	// add the pan

	stateParams.init(&apvts, getTotalNumInputChannels());

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
	// On synchronise le samplerate des classes avant de jouer
	this->stateParams.setSampleRate(sampleRate);
	//this->stateParams.getNumChannels(getTotalNumInputChannels()) // préférer ça que passer la variable
	grainEngine.init(sampleRate, getTotalNumOutputChannels(), samplesPerBlock);

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
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void ParticulesAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	//juce::ScopedNoDenormals noDenormals;
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples(); // buffer size

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, numSamples);

	//adsrParameters.attack = stateParams->getAttack();
	//adsrParameters.decay = stateParams->getDecay();
	//adsrParameters.sustain = stateParams->getSustain();
	//adsrParameters.release = stateParams->getRelease();

	//for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
	//	juce::ADSR::applyEnvelopeToBuffer(buffer, channel, numSamples);
	//}

	// let the engine operate under the given buffer
	bool loaded = stateParams.getAudioLoaded()->getValue();
	if (loaded == true && stateParams.getIsPlaying() == true) {
		grainEngine.process(buffer, numSamples);
	}

	//for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	//{
	//	float* channelDataDry = buffer.getWritePointer(channel);
	//	float* channelDataWet = grainBuffer->getWritePointer(channel);

	//	// iterate for all the samples in the given buffer
	//	for (int samplePos = 0; samplePos < numSamples; samplePos++) {

	//	}
	//}


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

	if (xmlState.get() != nullptr) {
		// si on trouve un fichier xml alors on met à jours les données dans apvts
		if (xmlState->hasTagName(apvts.state.getType()))
			apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
	}
	else {
		// on initialise tout avec les valeurs de base
		initValueTreeState();
	}

	//juce::Logger::outputDebugString("apvts mix : " + (juce::String)apvts.getRawParameterValue(MIX_ID)->load());
	//juce::Logger::outputDebugString("apvts gain : " + (juce::String)apvts.getRawParameterValue(GAIN_ID)->load());
	//juce::Logger::outputDebugString("apvts dens : " + (juce::String)apvts.getRawParameterValue(DENSITY_ID)->load());
	//juce::Logger::outputDebugString("apvts dur : " + (juce::String)apvts.getRawParameterValue(DURATION_ID)->load());
	//juce::Logger::outputDebugString("apvts pitch : " + (juce::String)apvts.getRawParameterValue(PITCH_ID)->load());

	//stateParams.print();
}

void ParticulesAudioProcessor::initValueTreeState() {
	apvts.getRawParameterValue(MIX_ID)->store(MIX_DEFAULT);
	apvts.getRawParameterValue(GAIN_ID)->store(GAIN_DEFAULT);
	apvts.getRawParameterValue(DENSITY_ID)->store(DENSITY_DEFAULT);
	apvts.getRawParameterValue(DURATION_ID)->store(DURATION_DEFAULT);
	apvts.getRawParameterValue(SPEED_ID)->store(SPEED_DEFAULT);
	apvts.getRawParameterValue(ENVTYPE_ID)->store(ENVTYPE_DEFAULT);
}

StateParameters* ParticulesAudioProcessor::getStateParameters() {
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
			MIX_ID,
			MIX_NAME,
			MIX_MIN, MIX_MAX, MIX_DEFAULT)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			GAIN_ID,
			GAIN_NAME,
			juce::NormalisableRange<float>(
				static_cast<float>(GAIN_MIN),
				static_cast<float>(GAIN_MAX),
				0.01f),
			static_cast<float>(GAIN_DEFAULT))
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			// id, name, min, max, init val
			DENSITY_ID,
			DENSITY_NAME,
			juce::NormalisableRange<float>(DENSITY_MIN, DENSITY_MAX, 0.001f),
			DENSITY_DEFAULT)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			DURATION_ID,
			DURATION_NAME,
			juce::NormalisableRange<float>(DURATION_MIN, DURATION_MAX, 0.001f),
			DURATION_DEFAULT)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			SPEED_ID,
			SPEED_NAME,
			SPEED_MIN, SPEED_MAX, SPEED_DEFAULT)
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
			POSITION_ID,
			POSITION_NAME,
			juce::NormalisableRange<float>(POSITION_MIN, POSITION_MAX, 0.001f),
			POSITION_DEFAULT)
	);

	layout.add(
		std::make_unique<juce::AudioParameterFloat>(
			SELECTION_ID,
			SELECTION_NAME,
			juce::NormalisableRange<float>(SELECTION_MIN, SELECTION_MAX, 0.001f),
			SELECTION_DEFAULT)
	);

	const juce::StringArray& choices{ ENVTYPE_1, ENVTYPE_2, ENVTYPE_3 };

	layout.add(
		std::make_unique<juce::AudioParameterInt>(
			ENVTYPE_ID, ENVTYPE_NAME, 1, 3, ENVTYPE_DEFAULT)
	);


	// ajouter envWidth, pan, direction 
	// randomDensity, randomDuration, randomPan, randomDirection, randomPitch

	return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ParticulesAudioProcessor();
}
