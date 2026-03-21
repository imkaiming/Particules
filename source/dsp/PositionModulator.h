/*
  ==============================================================================

	PhaseMod.h
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/
#include <juce_core/juce_core.h>

#include "../framework/Constants.h"
#include "../utils/enum/TraversalMode.h"

#include <random>
#pragma once

// unipolar LFO modulator return normalized position [0.f, 1.f]

// This class builds and holds table data
// it is also responsible of computing values based on the sample rate

class PositionModulator
{
public:
    PositionModulator();
    ~PositionModulator() = default;

    void setSampleRate(double sr);
    void setParameters(TraversalMode, float);

    void reset();
    void advanceBlock(int);
    const float computePhase();

    //float getPhaseAtOffset(int);
    //float getCurrentValue() const noexcept { return mCurrentValue; };

private:

    static constexpr int SIZE = LUT_SIZE;
    static constexpr int MODCOUNT = 4;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionModulator)
};