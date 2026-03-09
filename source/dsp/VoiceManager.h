/*
  ==============================================================================

	VoiceManager.h
	Created: 6 Feb 2026 6:03:01pm
	Author:  kai

  ==============================================================================
*/

#pragma once

#include "GrainPool.h"
#include "PositionModulator.h"
#include "../framework/GrainHandle.h"

// VoiceManager takes segments from the spawned events and render grains from it

struct ParameterSnapshot;
struct SampleSource;
class VoiceManager
{
public:
	explicit VoiceManager(GrainPool& pool, PositionModulator& pm);
	~VoiceManager() = default;

	void reset();
	void process(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource);
    void spawn(int offset, const ParameterSnapshot& snapshot);

private:
	void removeVoice(const int index);
	static constexpr int mCapacity = Param::MaxGrains;

    PositionModulator& posMod;
	GrainPool& pool;
	std::array<GrainHandle, mCapacity> activeHandles;
	uint16_t activeCount;
};