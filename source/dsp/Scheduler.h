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

// Scheduler is responsible of computing the events 

#pragma once


#include "../framework/Core.h"
#include "../utils/PhaseMod.h"
#include "../frameWork/ParameterSnapshot.h"
#include "../utils/GrainVisualizer.h"

// Scheduler compute onsets 

class Scheduler
{
public:
    explicit Scheduler();
	~Scheduler() = default;

	void process(int bufferSize, double sampleRate, float density, std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& parameters);

private:
	static constexpr uint16_t mCapacity = (uint16_t)Param::MaxEvents;

	void reset() { nextOnSet = 0; }
	double getInterOnSet(float density, double sampleRate) const noexcept;
	double getOffset() const noexcept { return nextOnSet; };
	void setOffset(double n) noexcept { nextOnSet = n; };

	juce::Random random;		// parameters to set the interOnset

	double nextOnSet;				// Tells us when the next grain should play

	//juce::dsp::Oscillator<float> lfoTraversal;
	//PhaseMod phaseMod;

};