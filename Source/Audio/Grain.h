/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include <juce_dsp/juce_dsp.h>


using AudioBlock = juce::dsp::AudioBlock<float>;

enum WindowingMethod
{
	triangular,
	hann,
	hamming
};

//static float ncos(size_t order, size_t i, size_t size);

class Grain {

public:
	Grain(int, int, int, float, AudioBlock*);
	~Grain();
	//void setActive(bool);
	float getCurrentSample(int);
	bool isActive();
	void update(); // int);
	void applyCrossFade(int, bool);
	int remainingLife();
	void updateBuffer(AudioBlock*);
	void init();

private:

	float ncos(size_t, size_t, size_t);

	float parabolicEnvelope(int, int, float);
	float trapezoidalEnvelope(int, int, float);
	float raisedCosineBellEnvelope(int, int, float);

	float hammingEnvelope(int,  int) ;
	float triangularEnvelope(int,  int);
	float hannEnvelope(int, int);
	float applyEnvelope(float);
	WindowingMethod getWindowingMethod(int);
	WindowingMethod window;
	//GrainEnvelope grainEnv;



	//juce::dsp::WindowingFunction<float>::
	int currentTime;
	//std::vector<int> currentTimeChannel; // le temps courrant pour chaque channel
	bool active;		// on active ou désactive le grain entier 
	const int length;		// on compte la durée en nombre de sample
	const int numChannel;
	float speed;
	//const int numSamples;
	//const int envelopeType;	// on associe un grain a une envelope


	//AudioBlock* audioBlock; // on ne modifie pas le buffer mais on le lit seulement
	juce::AudioBuffer<float> buffer;
	//double startTime;
	//double frequency;
	//float amplitude;
	//double currentTime;

};

