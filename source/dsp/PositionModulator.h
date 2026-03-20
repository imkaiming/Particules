/*
  ==============================================================================

	PhaseMod.h
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/
#include <juce_core/juce_core.h>

#include "../framework/Core.h"
#include "../utils/TraversalMode.h"

#include <random>
#pragma once

// unipolar LFO modulator return normalized position [0.f, 1.f]

class PositionModulator
{
public:
    PositionModulator();
    ~PositionModulator() = default;

    void setSampleRate(double);
    void setParameters(TraversalMode, float);

    void reset();
    void advanceBlock(int);
    const float computePhase();

    //float getPhaseAtOffset(int);
    //float getCurrentValue() const noexcept { return mCurrentValue; };

    static constexpr int SIZE = 2048;
    static constexpr int MODCOUNT = 4;

private:

    // init tables
    void initTableData();
    void initTablePtr();

    float getUnipolarSine(float);
    float getUnipolarCos(float); // may not be used
    float getUnipolarTriangular(float);
    float getUnipolarSquare(float);
    //float getRandom() { return r.nextFloat(); };

    std::array<const float*, MODCOUNT> tables;
    std::array<float, SIZE> sineTable;
    std::array<float, SIZE> squareTable;
    std::array<float, SIZE> triangleTable;
    std::array<float, SIZE> randomTable;

    float mSampleRate;
    TraversalMode mTraversalMod; // the mod type to compute
    float mTraversalFreq;        // frequency of the traversal
    float mPhaseIncrement;       // time step per samples
    float mPhaseAccumulator;     // position of a bufferSize block samples

    juce::Random r;
};