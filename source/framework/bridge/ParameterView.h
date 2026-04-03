#pragma once

#include "../../utils/enum/EnvelopeMode.h"
#include "../../utils/enum/TraversalMode.h"
#include "../../utils/struct/ParameterSnapshot.h"
#include "../PluginTypes.h"

#include "../Core.h"

// APVTS wrapper that create snapshot for the DSP
// own atomic ptr

namespace particules
{
    class EngineState;
    class ParameterView
    {
    public:
        ParameterView(EngineState&); // = default;
        ~ParameterView() = default;

        void init(ValueTreeState& apvts);

        ParameterSnapshot getSnapshot() const noexcept;

    private:
        float getDecibelGain() const noexcept { return output ? output->load(std::memory_order_relaxed) : 0.0f; }
        float getLinearGain() const noexcept;
        float getSpeed() const noexcept { return speed ? speed->load(std::memory_order_relaxed) : 0.0f; }
        float getEmission() const noexcept { return emission ? emission->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedDuration() const noexcept { return duration ? duration->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedWindowSelection() const noexcept { return span ? span->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedStartPosition() const noexcept { return position ? position->load(std::memory_order_relaxed) : 0.0f; }
        float getNormalizedSustainRatio() const noexcept
        {
            return sustainRatio ? sustainRatio->load(std::memory_order_relaxed) : 0.0f;
        }
        float getTraversalFreq() const noexcept { return traversalFreq ? traversalFreq->load(std::memory_order_relaxed) : 0.0f; }
        float getPlay() const noexcept { return play ? play->load(std::memory_order_relaxed) : false; }

        EnvelopeMode getEnvelopeMode() const noexcept;
        TraversalMode getTraversalMode() const noexcept;

        std::atomic<float>* play = nullptr;
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

        EngineState& engineState;

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterView)
    };
}