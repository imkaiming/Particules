#pragma once

#include "../Core.h"
#include "../../utils/struct/VisualSnapshot.h"

// snapshot of every actives grains published every buffers end

namespace particules
{
    class GrainVisualBuffer
    {
    public:
        GrainVisualBuffer() = default;
        ~GrainVisualBuffer() = default;

        // audio thread
        VisualSnapshot& getSnapshot(int i) noexcept { return visualSnapshot[i]; };

        // gui thread
        const VisualSnapshot& getSnapshot() const noexcept { return visualSnapshot[readIndex.load(std::memory_order_acquire)]; }

        std::atomic<int>& getReadIndex() noexcept { return readIndex; };

    private:
        VisualSnapshot visualSnapshot[2];
        std::atomic<int> readIndex{0}; // act like a latch that avoid race condition from ui and audio thread

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainVisualBuffer)
    };
}