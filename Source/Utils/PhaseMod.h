/*
  ==============================================================================

	PhaseMod.h
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include <random>
#include "../framework/Core.h"
#pragma once

//#include <juce_core/juce_core.h>


enum Mod {
	Sinus,
	Triangular,
	Square,
	Random,
	None
};





class PhaseMod// : public juce::dsp::Phase<float>
{
public:
	//PhaseMod(double sampleRate, float frequency, int mod);
	//PhaseMod(double sampleRate);
	PhaseMod();
	~PhaseMod();

	void setSampleRate(double);
	void setFrequency(float);
	void setMod(int);

	float getFrequency();
	int getMod();

	void reset();
	void advance();
	float getValue();

private:

	float nextSine(float);
	float nextTriangular(float);
	float nextSquare(float);
	float nextRandom();
	void updateDelta();

	int mMod;			// the modulation switch for the function to compute
	double mSampleRate;
	float mFrequency;
	float mPhase;
	float mValue;
	float mDelta;		// frequency / sampleRate
	juce::Random r;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseMod)
};