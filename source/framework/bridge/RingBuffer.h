#pragma once
#include <array>
#include <atomic>

namespace particules
{
    // SPSC Queue spécialisée pour transporter des pointeurs (T*)
    template <typename T, size_t Size = 16>
    class LockFreePointerQueue
    {
    public:
        bool push(T* item) noexcept
        {
            const unsigned long long currentWrite = writePos.load(std::memory_order_relaxed);
            const unsigned long long nextWrite = (currentWrite + 1) % Size;

            if(nextWrite == readPos.load(std::memory_order_acquire))
                return false; // File pleine

            buffer[currentWrite] = item;
            writePos.store(nextWrite, std::memory_order_release);
            return true;
        }

        T* pop() noexcept
        {
            const unsigned long long currentRead = readPos.load(std::memory_order_relaxed);

            if(currentRead == writePos.load(std::memory_order_acquire))
                return nullptr; // File vide

            T* item = buffer[currentRead];
            readPos.store((currentRead + 1) % Size, std::memory_order_release);
            return item;
        }

    private:
        std::array<T*, Size> buffer{};
        std::atomic<size_t> writePos{0};
        std::atomic<size_t> readPos{0};
    };
}