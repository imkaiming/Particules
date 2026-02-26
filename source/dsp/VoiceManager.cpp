/*
  ==============================================================================

	VoiceManager.cpp
	Created: 6 Feb 2026 6:03:01pm
	Author:  kai

  ==============================================================================
*/

#include "VoiceManager.h"
#include "../framework/ParameterSnapshot.h"
#include "../framework/SampleSource.h"

VoiceManager::VoiceManager(GrainPool& p) : pool { p }, activeCount { 0 } { reset(); }

void VoiceManager::reset()
{
    activeCount = 0;
    for(GrainHandle& handle : activeHandles)
        handle = GrainHandle::getInvalidState();
    pool.reset();
}

void VoiceManager::process(AudioBlock& outputBlock, int bufferSize, const SampleSource* source)
{
    const int numChannels = outputBlock.getNumChannels();
    const int numSamples = outputBlock.getNumSamples();

    for(size_t currentSample = 0; currentSample < numSamples; currentSample++)
    {
        for(int i = activeCount - 1; i >= 0; --i) // backward iteration for removing handle securely
        {
            GrainHandle h = activeHandles[i];
            Grain* g = pool.get(h); 
            if(!g) // grain has been released already MAY NOT NEEDED
            {
                removeVoice(i); // place the current handle[index] in activeCount index and became
                continue; // restart the begining of the loop at the same index
            }

            for(uint16_t channel = 0; channel < numChannels; ++channel)
                outputBlock.addSample(channel, currentSample, g->getCurrentSample(source, channel, numChannels));
            g->update();
            if(g->isExhausted())
            {
                pool.release(h);
                removeVoice(i);
            }
        }
    }

    // TODO : proper AGC automatic gain compensation
    if(activeCount > 0)
    {
        float scale = 1 / std::sqrt(static_cast<float>(activeCount));
        outputBlock.multiplyBy(scale);
    }
}

void VoiceManager::spawn(int offset, const ParameterSnapshot& snapshot)
{
    if(activeCount >= mCapacity)
        return; // cannot spawn any more grains

    GrainHandle handle = pool.acquire();
    Grain* grain = pool.get(handle);
    if(grain == nullptr)
        return;

    grain->config(snapshot, offset); // init the grain here before process with the snapshot
    activeHandles[activeCount++] = handle;
}

// example : after spawning 5 times activeCount = 5
// removing index 2 then swapping the index 4 with the 2 and
// after decrementing activeCount is = 4.
void VoiceManager::removeVoice(uint16_t i)
{
    activeHandles[i] = activeHandles[--activeCount];
}
