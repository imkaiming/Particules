
#pragma once
#include "EnvelopeLookUpTable.h"
#include "Grain.h"
#include <array>

// Preallocate Grains so we dont use new / delete in the audio thread
// use LIFO

struct GrainHandle;
class GrainPool
{
public:
    GrainPool();
    ~GrainPool() = default;

    Grain* get(const GrainHandle handle);
    GrainHandle acquire();
    void release(const GrainHandle hadle);
    void reset();

    int getNumActiveGrains() const noexcept { return nextFree; }
    bool isFull() const noexcept { return nextFree == SIZE; }
    bool isEmpty() const noexcept { return nextFree == 0; }

private:
    static constexpr int SIZE = MAX_GRAINS; 

    std::array<Grain, SIZE> grains;
    std::array<int, SIZE> freeIndices; // unordered stack of indexes. The most recently freed grain (also cache friendly)
    int nextFree = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainPool)
};
