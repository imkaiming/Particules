#include "PluginProcessor.h"
#include "PluginEditor.h"

ParticulesAudioProcessor::ParticulesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", createParameters()), granSynth(&stateSaver)
#endif
{
	//for (int i = 0; i < mNumVoices; ++i) {
	//	granSynth.addVoice(new juce::SamplerVoice());
	//}
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
	juce::Logger::outputDebugString("on configure le sample rate a : " + (juce::String)sampleRate);

	granSynth.prepareToPlay(sampleRate, samplesPerBlock);
	this->sampleRate = sampleRate;
	this->stateSaver.setSampleRate(sampleRate);

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
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();
	int currentBufferLength = buffer.getNumSamples();

	// dans le cas où on a plus de channel out que de channel in
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, currentBufferLength);

	granSynth.processBlock(buffer, midiMessages, totalNumOutputChannels);

}

bool ParticulesAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ParticulesAudioProcessor::createEditor()
{
	return new ParticulesAudioProcessorEditor(*this);
}

void ParticulesAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ParticulesAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ParticulesAudioProcessor();
}

StateSaver* ParticulesAudioProcessor::getStateSaver()
{
	return &stateSaver;
}

juce::AudioBuffer<float>* ParticulesAudioProcessor::getBuffer() {
	return &buffer;
}

juce::AudioProcessorValueTreeState::ParameterLayout ParticulesAudioProcessor::createParameters() {
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	// add parameters here
	params.push_back(
		std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.5f)
	);

	return { params.begin(), params.end() };
}