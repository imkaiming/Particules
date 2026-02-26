/*
  ==============================================================================

	PhaseMod.h
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include "../framework/Core.h"
#include <random>
#pragma once

enum Mod {
    Sinus,
    Triangular,
    Square,
    Random,
    None
};

class PhaseMod
{
public:
    PhaseMod();
    ~PhaseMod() = default;

    void setSampleRate(double);
    void setFrequency(float);
    void setMod(int newValue) { mMod = newValue; }

    void reset();
    void advance();

    float getValue() const noexcept { return mValue; };
    float getFrequency() const noexcept { return mFrequency; }
    int getMod() const noexcept { return mMod; }

private:
    float nextSine(float);
    float nextTriangular(float);
    float nextSquare(float);
    float nextRandom() { return r.nextFloat(); };
    void updateDelta();

    int mMod; // the modulation switch for the function to compute
    double mSampleRate = 0.0;
    float mFrequency = 0.f;
    float mPhase = 0.f;
    float mValue = 0.f;
    float mDelta = 0.f; // frequency / sampleRate
    juce::Random r;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseMod)
};