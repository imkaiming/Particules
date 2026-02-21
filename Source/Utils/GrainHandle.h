/*
  ==============================================================================

	GrainHandle.h
	Created: 16 Feb 2026 6:58:26pm
	Author:  kai

  ==============================================================================
*/

#pragma once

//#include "../Audio/Grain.h"
#include "../Framework/Core.h"

struct GrainHandle
{
	uint16_t index = 0xFFFF;
	uint16_t gen = 0; // to detect stale handles
	bool isValid() const { return index != 0xFFFF; };
	void invalidate() noexcept
	{
		index = 0;
		gen = 0;
	}
};