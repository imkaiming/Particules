/*
  ==============================================================================

	GrainHandle.h
	Created: 16 Feb 2026 6:58:26pm
	Author:  kai

  ==============================================================================
*/

#pragma once

#include "../Framework/Core.h"

struct GrainHandle
{
    uint16_t index = 0xFFFF;
    uint16_t gen = 0;
    GrainHandle() = default;
    GrainHandle(uint16_t i, uint16_t g) : index { i }, gen { g } {};
    bool isValid() const noexcept { return index != 0xFFFF; };
    static GrainHandle getInvalidState() noexcept { return { 0xFFFF, 0 }; };
};