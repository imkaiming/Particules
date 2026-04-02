#pragma once
#include "../../utils/struct/EngineSnapshot.h"
#include "../Core.h"
// own runtime flags
// audio processor can write
// plugin editor reads
namespace particules
{
    class EngineState
    {
    public:
        EngineState();
        ~EngineState() = default;

        const EngineSnapshot getSnapshot() const noexcept;

        //void setIsPlaying(bool b) noexcept { isPlaying.store(b, std::memory_order_relaxed); }
        void setIsGrainsEmpty(bool b) noexcept { isGrainsEmpty.store(b, std::memory_order_relaxed); }
        void setNumChannels(int ch) noexcept { numChannels.store(ch, std::memory_order_relaxed); }
        void setNumSamples(int s) noexcept { numSamples.store(s, std::memory_order_relaxed); }
        void setSampleRate(double sr) noexcept { sampleRate.store(sr, std::memory_order_relaxed); }
        void setNumActiveGrains(int g) noexcept { numActiveGrains.store(g, std::memory_order_relaxed); }

        //bool getIsPlaying() const noexcept { return isPlaying.load(std::memory_order_relaxed); }
        bool getIsGrainsEmpty() const noexcept { return isGrainsEmpty.load(std::memory_order_relaxed); }
        int getNumChannels() const noexcept { return numChannels.load(std::memory_order_relaxed); }
        int getNumSamples() const noexcept { return numSamples.load(std::memory_order_relaxed); }
        double getSampleRate() const noexcept { return sampleRate.load(std::memory_order_relaxed); }
        int getNumActiveGrains() const noexcept { return numActiveGrains.load(std::memory_order_relaxed); }

    private:
        //std::atomic<bool> isPlaying;
        std::atomic<bool> isGrainsEmpty;
        std::atomic<double> sampleRate;
        std::atomic<int> numChannels;
        std::atomic<int> numSamples;
        std::atomic<int> numActiveGrains;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineState)
    };
}