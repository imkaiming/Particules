/*
  ==============================================================================

	Scheduler.h
	Created: 15 Feb 2023 1:44:18pm
	Author:  user

  ==============================================================================
*/

// Maintains state necessary for activating grains according to Grain onset timesand durations
// Exposes a method for synthesizing samples of sound by mixing together the output from its active Grains.
// Manages grain allocation, for efficiency it may maintain a pool of reusable Grains

#pragma once

#include "../Utils/StateParameters.h"
#include "Grain.h"
#include "../Utils/PhaseMod.h"

using AudioBlock = juce::dsp::AudioBlock<float>;
using Buffer = juce::AudioBuffer<float>;

class Scheduler
{
public:
	Scheduler(StateParameters*); // , juce::AudioBuffer<float>*);
	~Scheduler();
	void synthesize(AudioBlock*, int, int); // , juce::AudioBuffer<float>*);
	Grain* generateGrain(int);
	void init(int);

private:
	void freeActiveGrains();

	StateParameters* stateParams;

	juce::Array<Grain*> grains; // Contient que des grains actifs en devenir d'être inactif

	juce::Random random;		// parameters to set the interOnset
	int nextOnset;				// Tells us when the next grain should play
	int nbActiveGrains;
	int numChannels;

	//juce::dsp::Oscillator<float> lfoTraversal;
	PhaseMod phaseMod;
};