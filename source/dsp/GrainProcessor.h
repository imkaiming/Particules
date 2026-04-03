#pragma once

#include "../framework/Constants.h"
#include "../framework/Core.h"
#include "../utils/struct/GrainHandle.h"

// GrainProcessor takes segments from the spawned events and render grains from it
namespace particules
{
    class GrainPool;
    struct SmoothedParameters;
    struct ParameterSnapshot;
    class GrainVisualBuffer;
    class GrainEnvelope;
    class PositionModulator;
    class GrainProcessor
    {
    public:
        GrainProcessor(GrainPool& pool, PositionModulator& pm, GrainEnvelope& lut, GrainVisualBuffer& vb);
        ~GrainProcessor() = default;

        void reset();
        //void render(const int, const int, AudioBlock&, const AudioBuffer*, const SmoothedParameters&);
        void render(int currentSample, int outputNumChannels, float* const* outputPtrs, const float* const* inputPtrs,
            const SmoothedParameters& params);

        //void process(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
        //void processGrainsSamples(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
        //void processSamplesGrains(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
        void spawn(const ParameterSnapshot&);

        void writeVisualSnapshot();

    private:
        void removeVoice(const int index);

        static constexpr int SIZE = global ::maxGrains;

        GrainEnvelope& envLut;
        PositionModulator& posMod;
        GrainPool& pool;

        std::array<GrainHandle, SIZE> activeHandles;
        int activeCount;

        GrainVisualBuffer& visualBuffer;
        std::array<float, SIZE> visualY;

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainProcessor)
    };
}