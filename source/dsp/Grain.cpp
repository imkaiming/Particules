/*
  ==============================================================================

	Grain.cpp
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#include "Grain.h"
#include "../utils/ParameterSnapshot.h"
#include "../utils/SmoothedParameters.h"

Grain::Grain()
    : durationSamples{0}, startPositionSamples{0}, speed{1.f}, sustainWidthSamples{0}, selectionWindow{0}, fadeInSamples{0},
      fadeOutSamples{0}, elapsedSamples{0}, delaySamples{0}, readPosition{0.f}, inputNumChannels{0},
      inputNumSamples{0} //, isInitialized{false} //, linearGain{1.f}
{
    reset();
}

void Grain::reset()
{
    durationSamples = 0;
    startPositionSamples = 0;
    sustainWidthSamples = 0;
    elapsedSamples = 0;
    fadeInSamples = 0;
    fadeOutSamples = 0;
    delaySamples = 0;
    generation = 0;
    inputNumSamples = 0;
    //linearGain = 1.f;
    speed = 1.f;
    readPosition = 0.f;
}

void Grain::config(const ParameterSnapshot& snapshot, int delay, float normalizedPosMod)
{
    elapsedSamples = 0;

    durationSamples = snapshot.durationSamples;
    inputNumSamples = snapshot.inputNumSamples;
    inputNumChannels = snapshot.inputNumChannels;

    // position data
    const int positionModulationSamples = static_cast<int>(normalizedPosMod * snapshot.selectionSamples);
    startPositionSamples = (snapshot.startPositionSamples + positionModulationSamples) % inputNumSamples;
    readPosition = static_cast<float>(startPositionSamples);
    selectionWindow = snapshot.selectionSamples;

    speed = snapshot.speed;
    delaySamples = delay;

    // envelope data
    //setEnvelopeData(static_cast<int>(snapshot.sustainRatio));

    //linearGain = snapshot.linearGain;
    sustainWidthSamples = static_cast<int>(snapshot.sustainRatio * durationSamples);
    fadeInSamples = static_cast<int>(0.5f * (durationSamples - sustainWidthSamples));
    fadeOutSamples = fadeInSamples + sustainWidthSamples;
    fadeInSamples = std::max<int>(fadeInSamples, 1);
    fadeOutSamples = std::max<int>(fadeOutSamples, 1);
    invFadeInSamples = 1.f / static_cast<float>(fadeInSamples);
}

//void Grain::setEnvelopeData(const float sustainRatio) noexcept {}

const float Grain::getCurrentSample(const AudioBuffer* inputbuffer, const int outChannel, const int outNumChannels) noexcept
{
    if(delaySamples > 0)
        return 0.f;

    // TODO mettre ca dans le VM idealement ou creer un ChannelManager
    const int intputNumsChannels = inputbuffer->getNumChannels();
    const float* sample = inputbuffer->getReadPointer(outChannel % intputNumsChannels);

    int index = static_cast<int>(readPosition);
    float frac = readPosition - (float)index;
    const float s0 = sample[index];
    const float s1 = sample[(index + 1) % inputNumSamples];

    return lerp(s0, s1, frac);
}

void Grain::updateParams(const SmoothedParameters& params) noexcept
{
    speed = params.speed;
    //setEnvelopeData(params.sustainRatio);
}

void Grain::nextReadPosition() noexcept
{
    if(delaySamples-- > 0)
        return;

    elapsedSamples++;
    readPosition += speed;
    while(readPosition >= static_cast<float>(inputNumSamples))
        readPosition -= static_cast<float>(inputNumSamples);
}

// EnvelopeLookUpTable
// fadeIn  : 0 - 0.5f
// sustain : 0.5f
// fadeOut : 0.5f - 1.f
const float Grain::getPhase() const noexcept
{
    if(durationSamples <= 0)
        return 0.f;

    const float elapsed = static_cast<float>(elapsedSamples);

    if(elapsed < fadeInSamples)
        return 0.5f * (elapsed * invFadeInSamples);
    else if(elapsed < fadeOutSamples)
        return 0.5f;
    else
    {
        const float stepsIntoRelease = elapsed - static_cast<float>(fadeOutSamples);
        const float releaseProgress = stepsIntoRelease * invFadeInSamples;
        return 0.5f + 0.5f * releaseProgress;
    }
}
