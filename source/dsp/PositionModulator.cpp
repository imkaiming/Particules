/*
  ==============================================================================

	PositionModulator.cpp
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

// quand on change le mod on reset la position de la phase.
// quand on met pause on reset egalement le modulator
// on calcule chaque avancer en fonction du bufferSize
#include "PositionModulator.h"

PositionModulator::PositionModulator(double sr)
    : mPhaseAccumulator{0.f}, mTraversalMod{TraversalMode::Sine}, mTraversalFreq{1.f}, mSampleRate{static_cast<float>(sr)}
{
    initTableData();
    initTablePtr();
}

void PositionModulator::initTableData()
{
    const float invSize = 1.f / (float)SIZE;
    for(int i = 0; i < SIZE; ++i)
    {
        const float val = static_cast<float>(i) * invSize;
        sineTable[i] = getUnipolarSine(val);
        squareTable[i] = getUnipolarSquare(val);
        triangleTable[i] = getUnipolarTriangular(val);
        randomTable[i] = r.nextFloat();
    }
}

void PositionModulator::initTablePtr()
{
    tables[static_cast<int>(TraversalMode::Sine)] = sineTable.data();
    tables[static_cast<int>(TraversalMode::Square)] = squareTable.data();
    tables[static_cast<int>(TraversalMode::Triangle)] = triangleTable.data();
    tables[static_cast<int>(TraversalMode::Random)] = randomTable.data();
}

void PositionModulator::reset() { mPhaseAccumulator = 0.f; }

void PositionModulator::setSampleRate(double sr)
{
    if(sr == 0)
    {
        jassert(sr != 0);
    }
    mSampleRate = static_cast<float>(sr);
}

void PositionModulator::setParameters(TraversalMode traversalMod, float traversalFreq)
{
    const float safeFreq = std::max(traversalFreq, 0.001f);

    mTraversalFreq = safeFreq;
    mPhaseIncrement = mTraversalFreq / mSampleRate;
    mTraversalMod = traversalMod;
}

void PositionModulator::advanceBlock(int numSamples)
{
    mPhaseAccumulator += mPhaseIncrement * numSamples;
    //mPhaseAccumulator = std::fmod(mPhaseAccumulator, 1.0f);
    if(mPhaseAccumulator >= 1.f)
        mPhaseAccumulator -= static_cast<int>(mPhaseAccumulator);
        //mPhaseAccumulator -= std::floor(mPhaseAccumulator);
}

const float PositionModulator::computePhaseAtOffset(int offset)
{
    if(mTraversalMod == TraversalMode::None)
        return 0.f;

    const float* table = tables[static_cast<int>(mTraversalMod)];

    float phase = mPhaseAccumulator + offset * mPhaseIncrement;
    phase -= static_cast<int>(phase);

    // TODO precompute table[i] and table[i+1] per buffer size if you want more perf
    //const float x = std::clamp(phase, 0.f, 1.f) * (static_cast<float>(SIZE) - 1.f);
    //const int i = std::min(static_cast<int>(x), SIZE - 2);
    //const float frac = x - i;

    //const float a = table[i];
    //const float b = table[i + 1];

    const int i = (int)(phase * SIZE) & (SIZE - 1);
    return table[i];
    //return a + frac * (b - a);
}
/*
float PositionModulator::getPhaseAtOffset(int offset)
{
    float phase = mPhaseAccumulator + offset * mPhaseIncrement;
    phase -= std::floor(phase);
    return phase;
}
*/
/*
float PositionModulator::computePhaseAtOffset(int offset)
{
    float normalizedPhase = getPhaseAtOffset(offset);
    float value = 0.f;
    switch(mTraversalMod)
    {
        case 1:
            value = getUnipolarSine(normalizedPhase);
            break;
        case 2:
            value = getUnipolarSquare(normalizedPhase);
            break;
        case 3:
            value = getUnipolarTriangular(normalizedPhase);
            break;
        case 4:
            value = getRandom();
            break;
        case 5:
            value = 0.f;
            break;
        default:
            std::logic_error("PositionModulator mTraversalMod should be 1 to 5");
            break;
    }
    return value;
}
*/

// mod types

float PositionModulator::getUnipolarCos(float normalizedPhase) { return 0.5f * (std::cos(twoPi * normalizedPhase) + 1.f); }

float PositionModulator::getUnipolarSine(float normalizedPhase) { return 0.5f * (std::sin(twoPi * normalizedPhase) + 1.0f); }

float PositionModulator::getUnipolarTriangular(float normalizedPhase)
{
    const float p = normalizedPhase - std::floor(normalizedPhase);
    return 1.0f - std::fabs(p * 2.0f - 1.0f);
}

float PositionModulator::getUnipolarSquare(float normalizedPhase)
{
    const float p = normalizedPhase - std::floor(normalizedPhase);
    return (p >= 0.5f);
}

//float PositionModulator::getRandom()
//{
//    static std::mt19937 rng(std::random_device{}());
//    static std::uniform_real_distribution<float> dist(0.f, 1.f);
//    return dist(rng);
//}
