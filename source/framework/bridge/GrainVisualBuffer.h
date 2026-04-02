#pragma once

#include "../Constants.h"
#include "../Core.h"
#include "../../utils/struct/GrainVisual.h"
#include "../../utils/struct/VisualSnapshot.h"

// snapshot of every actives grains published every buffers end

namespace particules
{
    class GrainVisualBuffer
    {
    public:
        GrainVisualBuffer() = default;
        ~GrainVisualBuffer() = default;

        //static constexpr int SIZE = global::maxGrains;

        //struct VisualSnapshot
        //{
        //    std::array<GrainVisual, SIZE> grainVisuals{}; 
        //    int count = 0;
        //};

        // audio thread
        VisualSnapshot& getSnapshot(int i) noexcept { return visualSnapshot[i]; };

        // gui thread
        const VisualSnapshot& getSnapshot() const noexcept { return visualSnapshot[readIndex.load(std::memory_order_acquire)]; }

        std::atomic<int>& getReadIndex() noexcept { return readIndex; };

    private:
        VisualSnapshot visualSnapshot[2];
        std::atomic<int> readIndex{0}; // act like a latch that avoid race condition from ui and audio thread

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainVisualBuffer)
    };
}