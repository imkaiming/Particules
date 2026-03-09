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

PositionModulator::PositionModulator(double sr) : mPhaseAccumulator{0.f}, mTraversalMod{0}, mTraversalFreq{1.f}, mSampleRate{sr}
{
}

void PositionModulator::reset()
{
    mPhaseAccumulator = 0.f;
    //mCurrentValue = 0.f;
}

void PositionModulator::setSampleRate(double sr)
{
    if(sr == 0)
    {
        jassert(sr != 0);
    }
    mSampleRate = sr;
}

// 1 sec pour 48 000hz = 48 000 samples
void PositionModulator::setParameters(int traversalMod, float traversalFreq)
{
    float safeFreq = std::max(traversalFreq, 0.001f); // protect the DSP without throwing an exception

    mTraversalFreq = safeFreq;
    mPhaseIncrement = mTraversalFreq / mSampleRate;
    mTraversalMod = traversalMod;
}

void PositionModulator::advanceBlock(int numSamples)
{
    // on somme le phase acc avec le nombre de sample
    // et on wrap back si on depasse un certains seuil
    // equivalent a 2 pi.
    mPhaseAccumulator += mPhaseIncrement * numSamples;
    mPhaseAccumulator = std::fmod(mPhaseAccumulator, 1.0f);
    if(mPhaseAccumulator < 0.f)
        mPhaseAccumulator += 1.f;
}

float PositionModulator::getPhaseAtOffset(int offset)
{
    float phase = mPhaseAccumulator + offset * mPhaseIncrement;
    phase -= std::floor(phase);
    return phase;
}

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

// mod types

float PositionModulator::getUnipolarCos(float normalizedPhase)
{
    return 0.5f * (std::cos(twoPi * normalizedPhase) + 1.f);
}
float PositionModulator::getUnipolarSine(float normalizedPhase)
{
    return 0.5f * (std::sin(twoPi * normalizedPhase) + 1.0f);
}

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