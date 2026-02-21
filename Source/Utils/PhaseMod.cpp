/*
  ==============================================================================

	PhaseMod.cpp
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include "PhaseMod.h"
#include "../Framework/ParamsID.h"

PhaseMod::PhaseMod()
{
	reset();
}

PhaseMod::~PhaseMod()
{
}

void PhaseMod::setFrequency(float newValue)
{
	mFrequency = newValue;
	updateDelta();
}

void PhaseMod::setSampleRate(double newValue) {
	jassert(newValue != 0);
	mSampleRate = newValue;
}

void PhaseMod::setMod(int newValue)
{
	mMod = newValue;
}

void PhaseMod::updateDelta()
{
	mDelta = mFrequency / static_cast<float>(mSampleRate);
}

float PhaseMod::getFrequency()
{
	return mFrequency;
}

int PhaseMod::getMod()
{
	return mMod;
}

void PhaseMod::reset()
{
	mPhase = 0.f;
	mValue = 0.f;
}

void PhaseMod::advance()
{
	switch (mMod) {
	case 1:
		mValue = nextSine(mPhase);
		break;
	case 2:
		mValue = nextTriangular(mPhase);
		break;
	case 3:
		mValue = nextSquare(mPhase);
		break;
	case 4:
		mValue = nextRandom();
		break;
	case 5:
		mValue = 0;
		break;
	default:
		break;
	}

	mPhase += mDelta;
	while (mPhase >= twoPi)
		mPhase -= twoPi;
}

float PhaseMod::nextSine(float phase)
{
	float offset = pi / 2; // so it start à pos 0
	return (std::sin(juce::MathConstants<float>::twoPi * phase + offset) + 1) / 2;
}

float PhaseMod::nextTriangular(float phase)
{
	// 2|x - floor(x + 0.5)| 
	return 2 * std::abs(phase - std::floorf(phase + 0.5f));
}

float PhaseMod::nextSquare(float phase)
{
	return phase < (twoPi / 2) ? 0.f : 1.f;
}

float PhaseMod::nextRandom()
{
	return r.nextFloat();
}

float PhaseMod::getValue()
{
	return mValue;
}
