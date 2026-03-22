#pragma once

#include "../framework/Constants.h"
#include "../framework/Core.h"
#include "../framework/GrainVisualBuffer.h"
#include "../utils/struct/GrainHandle.h"
#include "../utils/struct/ParameterSnapshot.h"
#include "../utils/struct/SmoothedParameters.h"
#include "GrainEnvelope.h"
#include "GrainPool.h"
#include "PositionModulator.h"
// VoiceManager takes segments from the spawned events and render grains from it
namespace particules
{
    //struct ParameterSnapshot;
    //struct SmoothedParameters;
    class VoiceManager
    {
    public:
        VoiceManager(GrainPool& pool, PositionModulator& pm, GrainEnvelope& lut, GrainVisualBuffer& vb);
        ~VoiceManager() = default;

        void reset();
        //void render(const int, const int, AudioBlock&, const AudioBuffer*, const SmoothedParameters&);
        void render(int currentSample, int outputNumChannels, float* const* outputPtrs, const float* const* inputPtrs,
            int inputNumSamples, const SmoothedParameters& params);

        //void process(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
        //void processGrainsSamples(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
        //void processSamplesGrains(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
        void spawn(const ParameterSnapshot& snapshot);

        void writeVisualSnapshot();

    private:
        void removeVoice(const int index);

        static constexpr int SIZE = Grains ::maxGrains;

        GrainEnvelope& envLut;
        PositionModulator& posMod;
        GrainPool& pool;

        std::array<GrainHandle, SIZE> activeHandles;
        uint16_t activeCount;

        GrainVisualBuffer& visualBuffer;
        std::array<float, SIZE> visualY;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoiceManager)
    };
}