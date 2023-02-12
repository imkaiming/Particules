/**
 * This class is the main processing class of the plugin and is responsible for 
 * handling the audio processing and control information.
*/

#pragma once

#include <JuceHeader.h>
#include "GrainSynth/GranularSynth.h"
#include "Component/AudioFileComponent.h"


class GranularSynthesisAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
	, public juce::AudioProcessorARAExtension
#endif
{
public:
	GranularSynthesisAudioProcessor();
	~GranularSynthesisAudioProcessor() override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	//AudioFileComponent* getAudioFileComponent();
	StateSaver* getStateSaver();

private:

	// constructors are called automatically
	GranularSynth granSynth;
	//AudioFileComponent audioFileComponent;
	StateSaver stateSaver;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularSynthesisAudioProcessor)

};
