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
	float durationSample; 
	float density;
	float fadeInt;
	float fadeOut;
	float linearGain;
	float speed;
	float mix;
	float startPositionSample;
	float selectionSample;
    float sustainRatio;

	float envType;
	float traversalMode;
	float traversalTime;

	double sampleRate;
	bool isValid() const
	{
		return durationSample > 0 && speed > 0 && density >= 0;
	}
};