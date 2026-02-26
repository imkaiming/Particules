/*
  ==============================================================================

	GrainPool.h
	Created: 6 Feb 2026 6:01:42pm
	Author:  kai

  ==============================================================================
*/

#pragma once
#include "Grain.h"

#include "../frameWork/Core.h"
// Preallocate Grains so we dont use new / delete in the audio thread
// use LIFO

struct GrainHandle;
class GrainPool
{
public:
    GrainPool();
    ~GrainPool() = default;

    Grain* get(const GrainHandle handle);
    bool acquire(GrainHandle& outHandle, Grain*& outGrain);
    GrainHandle acquire();
    void release(const GrainHandle hadle);
    void reset();

    //uint16_t getNumActiveGrains() const noexcept { return numActive; }
    bool isFull() const noexcept { return nextFree - 1 == mCapacity; }
    bool isEmpty() const noexcept { return nextFree - 1 == 0; }

private:
    static constexpr uint16_t mCapacity = Param::MaxGrains; // max grain = 500, 2^16 = 65535 values, 2^8 = 256 not enough

    std::array<Grain, mCapacity> grains;
    std::array<uint16_t, mCapacity> freeIndices; // unordered stack of indexes. The most recently freed grain (also cache friendly)
    uint16_t nextFree = 0;
};
