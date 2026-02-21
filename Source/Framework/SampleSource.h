/*
  ==============================================================================

	SampleSource.h
	Created: 10 Feb 2026 10:21:27pm
	Author:  kai

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// this define all the core parameters to the loaded sample source where the synthesis occur
// this is not the snapshot

struct SampleSource
{
    const juce::AudioBuffer<float> inputBuffer;
    const int numSamples; // total duration in sample length
    const double sampleRate;
    const int numChannels;

    SampleSource(juce::AudioBuffer<float> b, double sr)
        : inputBuffer(std::move(b)),
        numSamples(inputBuffer.getNumSamples()),
        sampleRate(sr),
        numChannels(inputBuffer.getNumChannels())
    {
        jassert(numSamples > 0 && sampleRate > 0.0);
    }

    SampleSource(const SampleSource&) = delete;
    SampleSource& operator=(const SampleSource&) = delete;
};