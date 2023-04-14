/*
  ==============================================================================

	Parameters.h
	Created: 26 Feb 2023 12:38:46am
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include "ParamsID.h"
#include "../Audio/Grain.h"
#include "../Utils/GrainVisualizer.h"

#pragma once

//enum Playback {
//	forward, backward
//};



class StateParameters
{
public:
	StateParameters();
	~StateParameters();

	//void parameterChanged(const juce::String& parameterID, float newValue);// override;
	void init(ValueTreeState* apvts, int numChannels);
	void print();


	// setters
	void setSampleRate(double);
	void setInterOnSet();
	void setGain(float);
	void setDensity(float);
	void setDuration(float);
	void setMix(float);
	void setSpeed(float);
	void setEnvelopeType(int);
	void setAudioLoaded(bool);
	void setAudioBuffer(juce::AudioBuffer<float>*);
	void setIsPlaying(bool);
	void setWindowSelection(float);
	void setFilePosition(float);
	void setEnvWidth(float);
	void setNumSamples(float);
	void setIsGrainsEmpty(bool);
	void setTraversalModeValue(int);
	void setTraversalTimeValue(float);


	// getters
	float getGain();
	float getSpeed();
	float getDensity();
	float getDuration();
	double getSampleRate();
	int getInterOnset();
	float getMix();
	int getNumChannels();
	int getEnvelopeType();
	//int getNumSamples();
	juce::Value* getAudioLoaded();
	bool getIsPlaying();
	float getWindowSelection();
	float getFilePosition();
	float getEnvWidth();
	float getNumSamples();
	bool getIsGrainsEmpty();
	int getTraversalModeValue();
	float getTraversalTimeValue();
	juce::AudioBuffer<float>* StateParameters::getAudioBuffer();


	void setGrains(juce::Array<Grain*>* grains);
	void setGrainVisualizer(GrainVisualizer*);

	void updateGrainVisualizer();

	juce::Array<Grain*>* getGrains();
	GrainVisualizer* getGrainVisualizer();
	//void valueChanged(juce::Value& value) override;
private:

	// Listened Parameters
	float mix;
	float density;		// number of grains per second emitted
	float duration;		// grain duration in ms
	float speed;
	int envelopeType;
	float gain;
	float envWidth;


	// Parameters relative to the AudioProcessor
	double sampleRate;
	int numChannels;

	// Derived Parameter
	int interOnset;


	// all the parameters relative to the Audio File and its managment
	juce::Value isAudioLoaded; // listenable boolean 
	bool isPlaying;
	int numSamples;
	juce::AudioBuffer<float>* audioFileBuffer;
	float filePosition;
	int traversalMode; // this is added value from the position to the end of the window selected by the user
	float traversalTime;
	float windowSelection;
	bool isGrainsEmpty;
	//juce::String filePath;
	//Playback playback; // grain can play in reverse


	//GrainVisualizer* grainVisualizer;
	juce::Array<Grain*>* grains;
	GrainVisualizer* grainVisualizer;
	// Random parameters
	//juce::Random random; // random modulator to affect the parameters
};