/*
  ==============================================================================

	PhaseMod.cpp
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include "PhaseMod.h"

PhaseMod::PhaseMod() : mPhase { 0.f }, mValue { 0.f }, mMod { 0 }, mDelta { 0.f }, mFrequency { 0.f } {}

void PhaseMod::reset()
{
    mPhase = 0.f;
    mValue = 0.f;
}

void PhaseMod::setFrequency(float newValue)
{
    mFrequency = newValue;
    updateDelta();
}

void PhaseMod::setSampleRate(double newValue)
{
    jassert(newValue != 0);
    mSampleRate = newValue;
}

void PhaseMod::updateDelta() { mDelta = mFrequency / static_cast<float>(mSampleRate); }

void PhaseMod::advance()
{
    switch(mMod)
    {
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
            std::logic_error("Phase Mod mValue should be 1 to 5");
            break;
    }

    mPhase += mDelta;
    while(mPhase >= twoPi)
        mPhase -= twoPi;
}

float PhaseMod::nextSine(float phase)
{
    float offset = pi / 2; // so it start à pos 0
    return (std::sin(juce::MathConstants<float>::twoPi * phase + offset) + 1) / 2;
}

float PhaseMod::nextTriangular(float phase) { return 2 * std::abs(phase - std::floorf(phase + 0.5f)); }

float PhaseMod::nextSquare(float phase) { return phase < (twoPi / 2) ? 0.f : 1.f; }
