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
	float duration;
	float density;
	float fadeInt;
	float fadeOut;
	float gain;
	float speed;
	float mix;
	float position;
	float selection;
	float envWidth;

	float envType;
	float traversalMode;
	float traversalTime;

	bool isValid() const
	{
		return duration > 0 && speed > 0 && density >= 0;
	}
};