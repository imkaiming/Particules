
#pragma once
#include "EnvelopeLookUpTable.h"
#include "Grain.h"
#include "../frameWork/Core.h"

// Preallocate Grains so we dont use new / delete in the audio thread
// use LIFO

struct GrainHandle;
class GrainPool
{
public:
    explicit GrainPool();
    ~GrainPool() = default;

    Grain* get(const GrainHandle handle);
    GrainHandle acquire();
    void release(const GrainHandle hadle);
    void reset();

    int getNumActiveGrains() const noexcept { return nextFree; }
    bool isFull() const noexcept { return nextFree == mCapacity; }
    bool isEmpty() const noexcept { return nextFree == 0; }

private:
    static constexpr int mCapacity = Param::MaxGrains; // max grain = 500, 2^16 = 65535 values, 2^8 = 256 not enough

    std::array<Grain, mCapacity> grains;
    std::array<int, mCapacity> freeIndices; // unordered stack of indexes. The most recently freed grain (also cache friendly)
    int nextFree = 0;
};
