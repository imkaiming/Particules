/*
  ==============================================================================

	VoiceManager.h
	Created: 6 Feb 2026 6:03:01pm
	Author:  kai

  ==============================================================================
*/

#pragma once

#include "GrainPool.h"
#include "../framework/GrainHandle.h"

// VoiceManager takes segments from the spawned events and render grains from it

struct ParameterSnapshot;
struct SampleSource;
class VoiceManager
{
public:
	explicit VoiceManager(GrainPool& pool);
	~VoiceManager() = default;

	void reset();
	void process(AudioBlock& outputBlock, int bufferSize, const SampleSource* source);
	void spawn(int offset, const ParameterSnapshot& snapshot);

private:
	void removeVoice(uint16_t index);
	static constexpr uint16_t mCapacity = Param::MaxGrains;

	GrainPool& grainPool;
	std::array<GrainHandle, mCapacity> activeHandles;
	uint16_t activeCount;
};