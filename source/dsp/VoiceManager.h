/*
  ==============================================================================

	VoiceManager.h
	Created: 6 Feb 2026 6:03:01pm
	Author:  kai

  ==============================================================================
*/

#pragma once

#include "../framework/GrainVisualBuffer.h"
#include "../utils/GrainHandle.h"
#include "EnvelopeLookUpTable.h"
#include "GrainPool.h"
#include "PositionModulator.h"

// VoiceManager takes segments from the spawned events and render grains from it

struct ParameterSnapshot;
class VoiceManager
{
public:
    explicit VoiceManager(GrainPool& pool, PositionModulator& pm, EnvelopeLookUpTable& lut, GrainVisualBuffer& vb);
    ~VoiceManager() = default;

    void reset();
    void render(const int, const int, AudioBlock&, const AudioBuffer*);
    //void process(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
    //void processGrainsSamples(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
    //void processSamplesGrains(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
    void spawn(int offset, const ParameterSnapshot& snapshot);

    void writeVisualSnapshot();

private:
    void removeVoice(const int index);

    static constexpr int SIZE = Param::MaxGrains;

    EnvelopeLookUpTable& envLut;
    PositionModulator& posMod;
    GrainPool& pool;

    std::array<GrainHandle, SIZE> activeHandles;
    uint16_t activeCount;

    GrainVisualBuffer& visualBuffer;
    std::array<float, SIZE> visualY;
};