/*
  ==============================================================================

	Snapshot.h
	Created: 16 Feb 2026 7:04:12pm
	Author:  kai

  ==============================================================================
*/

#pragma once

struct ParameterSnapshot
{
    int durationSamples;
    int startPositionSamples;
    int selectionSamples;
    int traversalMode;

    int inputNumSamples;
    int inputNumChannels;

    float emission;
    float linearGain;
    float speed;
    float mix;
    float sustainRatio;
    float envType;
    float traversalFreq;

    double sampleRate;
    bool isValid() const { return durationSamples > 0 && speed > 0 && emission >= 0; }
};