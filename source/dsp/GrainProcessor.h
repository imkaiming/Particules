#pragma once

#include "framework/core/Core.h"
#include "framework/core/PluginParams.h"
#include "utils/struct/GrainHandle.h"

// GrainProcessor takes segments from the spawned events and render grains from it
namespace particules
{
    class GrainPool;
    struct SmoothedParameters;
    struct ParameterSnapshot;
    class GrainEnvelope;
    class PositionModulator;
    struct VisualSnapshot;
    class GrainProcessor
    {
    public:
        GrainProcessor(GrainPool& pool, PositionModulator& pm, GrainEnvelope& lut);
        ~GrainProcessor() = default;

        void reset() noexcept;
        void render(int currentSample, int outputNumChannels, float* const* outputPtrs, const float* const* inputPtrs,
            const SmoothedParameters& params, int numSamples);

        void spawn(const ParameterSnapshot&);
        void writeVisualSnapshot(VisualSnapshot& snap) noexcept;

    private:
        void removeVoice(const int index);

        static constexpr int SIZE = params::maxActiveGrains;

        GrainEnvelope& envLut;
        PositionModulator& posMod;
        GrainPool& pool;

        std::array<GrainHandle, SIZE> activeHandles;
        int activeCount;

        std::array<float, SIZE> visualY;
    };
}