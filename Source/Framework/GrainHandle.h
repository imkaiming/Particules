/*
  ==============================================================================

	GrainHandle.h
	Created: 16 Feb 2026 6:58:26pm
	Author:  kai

  ==============================================================================
*/

#pragma once

//#include "../dsp/Grain.h"

struct GrainHandle
{
	uint16_t index = 0xFFFF;
	uint16_t gen = 0;
	bool isValid() const { return index != 0xFFFF; };
	static GrainHandle getInvalidState() { return {}; };
};