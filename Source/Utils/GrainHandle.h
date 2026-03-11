/*
  ==============================================================================

	GrainHandle.h
	Created: 16 Feb 2026 6:58:26pm
	Author:  kai

  ==============================================================================
*/

#pragma once

struct GrainHandle
{
    uint16_t index = 0xFFFF;
    uint16_t gen = 0;
    GrainHandle(uint16_t i, uint16_t g) : index { i }, gen { g } {};
    GrainHandle() = default;
    bool isValid() const noexcept { return index != 0xFFFF; };
    static GrainHandle getInvalidState() noexcept { return { 0xFFFF, 0 }; };
};