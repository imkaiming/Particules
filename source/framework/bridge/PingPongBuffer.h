#pragma once

#include "../Core.h"

// latch SPSC 
// Ping Pong Buffer (double buffering)
// useful to display infos from the audio to the GUI thread

namespace particules
{
    template <typename T>
    class PingPongBuffer
    {
        static_assert(
            std::is_trivially_copyable<T>(), "PingPongBuffer<T> : T must be trivially copyable. No Vector, array or raw ptr");

    public:
        PingPongBuffer() = default;

        // Audio Thread ; single producer
        T& beginWriteBuffer() noexcept { return buffers[writeIndex]; }

        // swapping index value to confirm the data has been publish
        void endWriteBuffer() noexcept
        {
            readIndex.store(writeIndex, std::memory_order_release);
            writeIndex = 1 - writeIndex;
        }

        // gui thread : single consummer
        const T& getReadBuffer() const noexcept { return buffers[readIndex.load(std::memory_order_acquire)]; }

        PingPongBuffer(const PingPongBuffer&) = delete; // no copy constructor
        PingPongBuffer& operator=(const PingPongBuffer&) = delete; // no assignation
    private:
        T buffers[2];
        int writeIndex = 0; // strictly for the audio thread
        std::atomic<int> readIndex{0}; 
    };
}