/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


#pragma once

#include "Framework/ParameterView.h"
#include "Audio/GranularEngine.h"
#include "Framework/UIContext.h"
#include "Framework/AudioFileLoader.h"
#include "Utils/CustomLookAndFeel.h"


class ParameterView;
class GranularEngine;
class ParticulesAudioProcessor: public juce::AudioProcessor, public juce::ChangeBroadcaster
#if JucePlugin_Enable_ARA
	, public juce::AudioProcessorARAExtension
#endif
{
public:
	ParticulesAudioProcessor();
	~ParticulesAudioProcessor() override;

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

	ValueTreeState& getValueTreeState() noexcept { return apvts; };
	ParameterView& getParametersView() noexcept { return paramsView; };
	UIContext& getUIContext() noexcept { return uiContext; };

	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	void loadFile(const juce::String& path);
	void loadFile();
	const juce::File& getCurrentFile() const noexcept { return currentFile; };
	AudioFileLoader& getAudioFileLoader() noexcept { return loader; };
private:

	void initOnAudioLoadedCallback();
	void loadDebugPreset();
	bool debugPresetLoaded = false;

	ValueTreeState apvts;			// connecte les slider du GUI et les paramètres (fourni des valeurs atomiques)
	ParameterView paramsView;	// fait le pont entre apvts et le synth
	GranularEngine grainEngine;		// le moteur de la synthèse granulaire


	CustomLookAndFeel customLookAndFeel;
	UIContext uiContext;

	AudioFileLoader loader;
	juce::File currentFile;
	std::function<void(const juce::File, bool)> onAudioLoadedCallback;
	//AudioBuffer inputBuffer;

	//juce::ADSR::Parameters adsrParameters;
	//juce::ADSR adsr;


	//juce::UndoManager undoManager;
	// juce::AudioSampleBuffer grainBuffer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessor)
};
