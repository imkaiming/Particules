/*
  ==============================================================================

	VoiceManager.cpp
	Created: 6 Feb 2026 6:03:01pm
	Author:  kai

  ==============================================================================
*/

#include "VoiceManager.h"
#include "../Framework/ParameterSnapshot.h"
#include "../Framework/SampleSource.h"

VoiceManager::VoiceManager(GrainPool& pool):grainPool{pool}, activeCount{0}
{
	reset();
}

void VoiceManager::reset()
{
	activeCount = 0;
	for(GrainHandle& handle : activeHandles) { handle = GrainHandle::getInvalidState(); };
	grainPool.reset();

}

void VoiceManager::process(AudioBlock& outputBlock, int bufferSize, const SampleSource* source)
{
	//source.get()->buffer;
	//source.get()->numChannels;
	//source.get()->numSamples;

	// on boucle en sample accurate
	// on saisie le grain avec le handle
	// on update sa position relative au buffer input
	// on ecrit dans le buffer output le resultat 
	// verifier les calculs d'enveloppe et des gains relatif

	for(size_t currentSample = 0; currentSample < bufferSize; ++currentSample)
	{
		for(size_t i = 0; i < activeCount;)
		{
			GrainHandle& handle = activeHandles[i];
			Grain* g = grainPool.get(handle);

			if(!g)
			{
				removeVoice(i); continue;
			}
			//buffer.getNumChannels();
			//source.get()->numChannels;
			for(int channel = 0; channel < outputBlock.getNumChannels(); ++channel)
			{
				//float* outputPtr = outputBlock.getWritePointer(channel);
				//outputPtr[currentSample] += g->getNextSample(source, channel, outputBlock.getNumChannels());
				outputBlock.addSample(channel, currentSample, g->getNextSample(source, channel, outputBlock.getNumChannels()));
				g->update();
			}
			if(!g->isExhausted())
			{
				grainPool.release(handle);
				removeVoice(i);
			} else
			{
				i++;
			}
		}
	}
}

void VoiceManager::spawn(int offset, const ParameterSnapshot& snapshot)
{
	if(activeCount >= mCapacity) return; // cannot spawn any more grains

	GrainHandle handle;
	Grain* grain = nullptr;
	if(!grainPool.acquire(handle, grain)) return;

	// init the grain here before process with the snapshot
	grain->config(snapshot, offset);
	grain->setActive(true);

	activeHandles[activeCount++] = handle;
}


// swap with last
void VoiceManager::removeVoice(uint16_t index)
{
	activeHandles[index] = activeHandles[--activeCount];
}
