#pragma once

#include "../Core.h"

#include <juce_core/juce_core.h>

// single source of truth for all bridges in the projects
// every component must be thread safe
// bridges are unidirectional from audio to gui

namespace particules
{
    class Meter
    {
    public:
        Meter() = default;
        ~Meter() = default;

        void pushRMS(float left, float right)
        {
            rmsL.store(left, std::memory_order_relaxed);
            rmsR.store(right, std::memory_order_relaxed);
        }
        void pushPeak(float left, float right)
        {
            peakL.store(left, std::memory_order_relaxed);
            peakR.store(right, std::memory_order_relaxed);
        }

        struct Frame
        {
            float rmsL, rmsR, peakL, peakR;
        };

        Frame loadFrame() const noexcept
        {
            return {rmsL.load(std::memory_order_relaxed), rmsR.load(std::memory_order_relaxed),
                peakL.load(std::memory_order_relaxed), peakR.load(std::memory_order_relaxed)};
        }

    private:
        std::atomic<float> rmsL;
        std::atomic<float> rmsR;
        std::atomic<float> peakL;
        std::atomic<float> peakR;
    };

}