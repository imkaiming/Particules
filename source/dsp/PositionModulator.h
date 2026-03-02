/*
  ==============================================================================

	PhaseMod.h
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/
#include <juce_core/juce_core.h>

#include "../framework/Core.h"

#include <random>
#pragma once

enum Mod { Sinus, Triangular, Square, Random, None };

// unipolar LFO modulator [0, 1[

class PositionModulator
{
public:
    PositionModulator(double);
    ~PositionModulator() = default;

    void setSampleRate(double);
    void setParameters(int, float);

    void reset();
    void advanceBlock(int);
    float getPhaseAtOffset(int);
    float computePhaseAtOffset(int);
    //float getCurrentValue() const noexcept { return mCurrentValue; };

private:
    //static constexpr const double twoPi = static_cast<double>(std::_Pi_val / 2.0);
    static constexpr const float twoPi = juce::MathConstants<float>().twoPi;
    static constexpr const float pi = juce::MathConstants<float>().pi;
    static constexpr const float halfPi = juce::MathConstants<float>().halfPi;

    float getSine(float);
    float getCos(float); // may not be used
    float getTriangular(float);
    float getSquare(float);
    float getRandom() { return r.nextFloat(); };

    double mSampleRate;
    int mTraversalMod;              // the mod type to compute
    float mTraversalFreq = 1.f;     // frequency of the traversal
    float mPhaseIncrement = 0.f;    // time step per samples
    //float mCurrentValue = 0.f;      //
    float mPhaseAccumulator = 0.f;  // position of a bufferSize block samples

    juce::Random r;
};