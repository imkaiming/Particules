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
    //jassert(sr != 0);
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
            value = getSine(normalizedPhase);
            break;
        case 2:
            value = getTriangular(normalizedPhase);
            break;
        case 3:
            value = getSquare(normalizedPhase);
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

float PositionModulator::getCos(float normalizedPhase)
{
    double radian = twoPi * normalizedPhase;
    return (std::sin(radian + halfPi) + 1) / 2;
}
float PositionModulator::getSine(float normalizedPhase)
{
    double radian = twoPi * normalizedPhase;
    return 0.5f * (std::sin(radian + halfPi) + 1.0f);
}

float PositionModulator::getTriangular(float normalizedPhase)
{
    return 2 * std::abs(normalizedPhase - std::floorf(normalizedPhase + 0.5f));
}

float PositionModulator::getSquare(float normalizedPhase) { return normalizedPhase < (0.5f) ? 0.f : 1.f; }
