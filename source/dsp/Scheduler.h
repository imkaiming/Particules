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

// explain what is a interOnsetSamples
//struct GrainEvent
//{
//	int offset;           // sample position inside the block itself (0..numSample-1)
//	//GrainConfig config; // parameters in the moment of the calculation(duration, startSample, speed, env, gain, etc.
//};

// Scheduler compute onsets 
class ParameterView;
class Scheduler
{
public:
	explicit  Scheduler(ParameterView& sp);
	~Scheduler() = default;


	// Block based scheduler - might switch to sample accurate scheduler later
	/*
	template<typename SpawnFn>
	void process(int bufferSize, double sampleRate, float density, SpawnFn&& spawn, const ParameterSnapshot& parameters)
	{
		const double interOnSet = getInterOnSet(density, sampleRate);
		if(interOnSet <= 1) { setOffset(0.0); jassert("interOnSet <= 1"); } // dont need 1 grain per sample this is too much

		int count = 0;
		double offset = getOffset(); // offset of the next outBuffer call
		while(offset < static_cast<double>(bufferSize) && count < mCapacity)
		{
			spawn(static_cast<int>(std::floor(offset)), parameters); // call the voice manager
			offset += interOnSet;
			count++;
		}

		setOffset(offset - bufferSize);
		//return count;
	};*/

	void process(int bufferSize, double sampleRate, float density, std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& parameters);

	void reset() { nextOnSet = 0; }

private:
	static constexpr uint16_t mCapacity = (uint16_t)Param::MaxEvents;

	double getInterOnSet(float density, double sampleRate) const noexcept;
	double getOffset() const noexcept { return nextOnSet; };
	void setOffset(double n) noexcept { nextOnSet = n; };

	ParameterView& paramsView;

	//juce::Array<Grain*> grains; // Contient que des grains actifs en devenir d'etre inactif

	juce::Random random;		// parameters to set the interOnset

	double nextOnSet;				// Tells us when the next grain should play

	//juce::dsp::Oscillator<float> lfoTraversal;
	//PhaseMod phaseMod;

};