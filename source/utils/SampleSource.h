/*
  ==============================================================================

	SampleSource.h
	Created: 10 Feb 2026 10:21:27pm
	Author:  kai

  ==============================================================================
*/

#pragma once

#include "Core.h"
// this define all the core parameters to the loaded sample source where the synthesis occur
// this is not the snapshot

struct SampleSource
{
    const AudioBuffer inputBuffer;
    const int numSamples;
    const int numChannels;

    SampleSource(AudioBuffer b)
        : inputBuffer(std::move(b)), numSamples(inputBuffer.getNumSamples()), numChannels(inputBuffer.getNumChannels())
    {
    }

    SampleSource(const SampleSource&) = delete;
    SampleSource& operator=(const SampleSource&) = delete;
};