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

	//void resetSmoothedGain(double, float);

	// setters
	void setSampleRate(double);
	void setInterOnSet();
	void setGain(float);
	//void setPreviousGain(float);
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
	juce::Value* getAudioLoaded();
	bool getIsPlaying();
	float getWindowSelection();
	float getFilePosition();
	juce::AudioBuffer<float>* StateParameters::getAudioBuffer();

	//void unloadBuffer();
	//void valueChanged(juce::Value& value) override;
private:

	// Listened Parameters
	float mix;			// [0.0; 1.0]
	float density;		// number of grains per second emitted
	float duration;		// grain duration in ms
	float speed;
	int envelopeType;
	float gain;


	// Parameters relative to the AudioProcessor
	double sampleRate;
	int numChannels;


	// Derived Parameter
	int interOnset;


	// all the parameters relative to the Audio File and its managment
	juce::Value isAudioLoaded; // listenable boolean 
	bool isPlaying;
	int audioFileNumSamples;
	juce::AudioBuffer<float>* audioFileBuffer;
	float filePosition; // starting point of the granulation in the file
	float windowSelection;
	//juce::String filePath;
	//Playback playback; // grain can play in reverse

	// Random parameters
	//juce::Random random; // random modulator to affect the parameters
};