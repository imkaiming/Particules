/*
  ==============================================================================

	PhaseMod.h
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include <random>
#pragma once



enum Mod {
	Sinus,
	Triangular,
	Square,
	Random,
	None
};

constexpr float twoPi = juce::MathConstants<float>::twoPi;
constexpr float pi = juce::MathConstants<float>::pi;



class PhaseMod// : public juce::dsp::Phase<float>
{
public:
	PhaseMod(double sampleRate, float frequency, int mod);
	PhaseMod(double sampleRate);
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