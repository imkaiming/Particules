#pragma once

#include "../utils/struct/GrainVisual.h"
#include "../framework/Constants.h"

// snapshot of every actives grains published every buffers end
// it suppose to mirror the GrainPools logic and should follow the exact grains

class GrainVisualBuffer
{
public:
    GrainVisualBuffer() = default;
    ~GrainVisualBuffer() = default;

    static constexpr int SIZE = MAX_GRAINS;

    struct VisualSnapshot
    {
        std::array<GrainVisual, SIZE> grainVisuals{}; // MaxGrains = 1024 par ex.
        int count = 0;
    };

    // audio thread
    VisualSnapshot& getSnapshot(int i) noexcept { return visualSnapshot[i]; };

    // gui thread
    const VisualSnapshot& getSnapshot() const noexcept { return visualSnapshot[readIndex.load(std::memory_order_acquire)]; }

    // exposing the atomic
    std::atomic<int>& getReadIndex() noexcept { return readIndex; };

private:
    VisualSnapshot visualSnapshot[2];
    std::atomic<int> readIndex{0}; // act like a latch

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainVisualBuffer)
};