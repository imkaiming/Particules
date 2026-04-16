#pragma once

#include "../Core.h"
#include "../../utils/struct/VisualSnapshot.h"

// snapshot of every actives grains published every buffers end
// Ping Pong Buffer (double buffering)
// single producer single consumer
// useful to display infos from the audio to the GUI thread

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

        std::atomic<int> readIndex{0}; // visible to GUI

    };
}