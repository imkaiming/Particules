#pragma once

#include "utils/enum/EnvelopeMode.h"
#include "utils/enum/TraversalMode.h"
#include "utils/struct/ParameterSnapshot.h"
#include "framework/core/PluginTypes.h"
#include "framework/core/Core.h"

// APVTS wrapper that create snapshot for the DSP
// own atomic ptr

namespace particules
{
    class AudioState;
    class ParameterState
    {
    public:
        ParameterState(AudioState&); 
        ~ParameterState() = default;

        void init(ValueTreeState& apvts);

        const ParameterSnapshot getSnapshot() const noexcept;

        float getPlay() const noexcept { return play ? play->load(std::memory_order_relaxed) : false; }

    private:
        float getDecibelGain() const noexcept { return output ? output->load(std::memory_order_relaxed) : 0.0f; }
        float getLinearGain() const noexcept;
        float getSpeed() const noexcept { return speed ? speed->load(std::memory_order_relaxed) : 0.0f; }
        float getEmission() const noexcept { return emission ? emission->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedDuration() const noexcept { return duration ? duration->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedSpan() const noexcept { return span ? span->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedStartPosition() const noexcept { return position ? position->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedSustainRatio() const noexcept
        {
            return sustainRatio ? sustainRatio->load(std::memory_order_relaxed) : 0.0f;
        }
        float getTraversalFreq() const noexcept { return traversalFreq ? traversalFreq->load(std::memory_order_relaxed) : 0.0f; }
        float getPlayback() const noexcept { return playback ? playback->load(std::memory_order_relaxed) : 0.0f; }

        EnvelopeMode getEnvelopeMode() const noexcept;
        TraversalMode getTraversalMode() const noexcept;

        std::atomic<float>* play = nullptr;
        std::atomic<float>* playback = nullptr;
        std::atomic<float>* emission = nullptr;
        std::atomic<float>* duration = nullptr;
        std::atomic<float>* speed = nullptr;
        std::atomic<float>* mix = nullptr;
        std::atomic<float>* output = nullptr;
        std::atomic<float>* position = nullptr;
        std::atomic<float>* span = nullptr;
        std::atomic<float>* sustainRatio = nullptr;
        std::atomic<float>* traversalFreq = nullptr;
        std::atomic<float>* traversalMode = nullptr;
        std::atomic<float>* envMode = nullptr;

        AudioState& audioState;

    };
}