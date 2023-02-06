#include "PluginProcessor.h"
#include "PluginEditor.h"

GranularSynthesisAudioProcessor::GranularSynthesisAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
}

GranularSynthesisAudioProcessor::~GranularSynthesisAudioProcessor()
{
}

const juce::String GranularSynthesisAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool GranularSynthesisAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool GranularSynthesisAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool GranularSynthesisAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double GranularSynthesisAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int GranularSynthesisAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int GranularSynthesisAudioProcessor::getCurrentProgram()
{
	return 0;
}

void GranularSynthesisAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String GranularSynthesisAudioProcessor::getProgramName(int index)
{
	return {};
}

void GranularSynthesisAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void GranularSynthesisAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	granSynth.prepareToPlay(sampleRate, samplesPerBlock);
}

void GranularSynthesisAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GranularSynthesisAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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



void GranularSynthesisAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();
	int currentBufferLength = buffer.getNumSamples();

	if (audioFileComponent.getCurrentState() != Stopped) {
		juce::Logger::outputDebugString("cannot launch granular synth when the audio player is running.");
		if (audioFileComponent.getReaderSource().get() == nullptr)
		{
			for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
				buffer.clear(i, 0, currentBufferLength);
			return;
		}

		audioFileComponent.getTransportSource().getNextAudioBlock(buffer);
	}



	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, currentBufferLength);
	// granSynth.processBlock(buffer, midiMessages);
}

bool GranularSynthesisAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GranularSynthesisAudioProcessor::createEditor()
{
	return new GranularSynthesisAudioProcessorEditor(*this);
}

void GranularSynthesisAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void GranularSynthesisAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new GranularSynthesisAudioProcessor();
}

AudioFileComponent* GranularSynthesisAudioProcessor::getAudioFileComponent()
{
	return &audioFileComponent;
}