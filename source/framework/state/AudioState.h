#pragma once
//#include "utils/struct/AudioStateSnapshot.h"
#include "framework/core/Core.h"

// own audio runtime flags
// audio thread only can write
// ui thread only reads by polling method
// it must remains :
// - trivially copyable types
// - lock‑free patterns
// - predictable memory access

namespace particules
{
    class AudioState
    {
    public:
        AudioState();
        ~AudioState() = default;

        //AudioStateSnapshot getSnapshot() const noexcept;

        //void setIsPlaying(bool b) noexcept { isPlaying.store(b, std::memory_order_relaxed); }
        void setIsGrainsEmpty(bool b) noexcept { isGrainsEmpty.store(b, std::memory_order_relaxed); }
        void setNumChannels(int ch) noexcept { numChannels.store(ch, std::memory_order_relaxed); }
        void setNumSamples(int s) noexcept { numSamples.store(s, std::memory_order_relaxed); }
        void setSampleRate(double sr) noexcept { sampleRate.store(sr, std::memory_order_relaxed); }
        void setNumActiveGrains(int g) noexcept { numActiveGrains.store(g, std::memory_order_relaxed); }
        void setLink(bool b) noexcept { isLinked.store(b, std::memory_order_relaxed); }
        void setIsAuditioning(bool b) noexcept { isAuditioning.store(b, std::memory_order_relaxed); }

        bool getIsGrainsEmpty() const noexcept { return isGrainsEmpty.load(std::memory_order_relaxed); }
        int getNumChannels() const noexcept { return numChannels.load(std::memory_order_relaxed); }
        int getNumSamples() const noexcept { return numSamples.load(std::memory_order_relaxed); }
        double getSampleRate() const noexcept { return sampleRate.load(std::memory_order_relaxed); }
        int getNumActiveGrains() const noexcept { return numActiveGrains.load(std::memory_order_relaxed); }
        bool getIsLinked() const noexcept { return isLinked.load(std::memory_order_relaxed); }
        bool getIsAuditioning() const noexcept { return isAuditioning.load(std::memory_order_relaxed); }

    private:
        std::atomic<bool> isAuditioning;
        std::atomic<bool> isGrainsEmpty;
        std::atomic<double> sampleRate;
        std::atomic<int> numChannels;
        std::atomic<int> numSamples;
        std::atomic<int> numActiveGrains;
        std::atomic<bool> isLinked;
    };
}