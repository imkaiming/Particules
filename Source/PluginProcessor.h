/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "Utils/StateParameters.h"
#include "Utils/ParamsID.h"
#include "Audio/GranularEngine.h"

class ParticulesAudioProcessor : public juce::AudioProcessor
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

	StateParameters* getStateParameters();
	ValueTreeState* getValueTreeState();
	//void initValueTreeState();
	//void setParameters(juce::AudioProcessorValueTreeState& apvts);
	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:

	ValueTreeState apvts;			// connecte les slider du GUI et les paramètres (fourni des valeurs atomiques)
	StateParameters stateParams;	// fait le pont entre apvts et le synth
	GranularEngine grainEngine;		// le moteur de la synthèse granulaire

	//juce::ADSR::Parameters adsrParameters;
	//juce::ADSR adsr;


	//juce::UndoManager undoManager;
	// juce::AudioSampleBuffer grainBuffer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessor)
};
