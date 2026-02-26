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

VoiceManager::VoiceManager(GrainPool& pool) : grainPool { pool }, activeCount { 0 } { reset(); }

void VoiceManager::reset()
{
    activeCount = 0;
    for(GrainHandle& handle : activeHandles)
        handle = GrainHandle::getInvalidState();
    grainPool.reset();
}

//source.get()->buffer;
//source.get()->numChannels;
//source.get()->numSamples;

// on boucle en sample accurate
// on saisie le grain avec le handle
// on update sa position relative au buffer input
// on ecrit dans le buffer output le resultat
// verifier les calculs d'enveloppe et des gains relatif
void VoiceManager::process(AudioBlock& outputBlock, int bufferSize, const SampleSource* source)
{
    int numChannels = outputBlock.getNumChannels();
    for(size_t currentSample = 0; currentSample < bufferSize; ++currentSample)
    {
        for(size_t i = 0; i < activeCount;)
        {
            GrainHandle& handle = activeHandles[i];
            Grain* g = grainPool.get(handle);

            if(!g)
            {
                removeVoice(i);
                continue;
            }
            for(int channel = 0; channel < numChannels; ++channel)
                outputBlock.addSample(channel, currentSample, g->getNextSample(source, channel, numChannels));

            g->update();

            if(g->isExhausted())
            {
                grainPool.release(handle);
                removeVoice(i);
            }
            else
            {
                i++;
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

    GrainHandle handle;
    Grain* grain = nullptr;
    if(!grainPool.acquire(handle, grain))
        return; // if max pool capacity has been reached
    grain->config(snapshot, offset); // init the grain here before process with the snapshot
    activeHandles[activeCount++] = handle;
}

// swap with last
void VoiceManager::removeVoice(uint16_t index)
{
    activeHandles[index] = activeHandles[--activeCount];
}
