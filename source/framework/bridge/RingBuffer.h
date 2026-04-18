#pragma once
#include "framework/core/Core.h"

// SPSC Queue (FIFO)
// its move pointers between thread
// gonna be used for SPSC pattern
// specially for moving AudioBuffer
// incoming queue and garbage collector

namespace particules
{
    template <typename T, size_t Size = 16>
    class RingBuffer
    {
    public:
        RingBuffer() = default;

        bool push(T* item) noexcept
        {
            const int currentWrite = writePos.load(std::memory_order_relaxed);
            const int nextWrite = (currentWrite + 1) % Size;

            if(nextWrite == readPos.load(std::memory_order_acquire))
                return false; // queue is full

            buffer[currentWrite] = item;
            writePos.store(nextWrite, std::memory_order_release);
            return true;
        }

        T* pop() noexcept
        {
            const int currentRead = readPos.load(std::memory_order_relaxed);

            if(currentRead == writePos.load(std::memory_order_acquire))
                return nullptr; // queue is empty

            T* item = buffer[currentRead];
            readPos.store((currentRead + 1) % Size, std::memory_order_release);
            return item;
        }

    private:
        std::array<T*, Size> buffer{};
        std::atomic<int> writePos{0};
        std::atomic<int> readPos{0};
    };
}