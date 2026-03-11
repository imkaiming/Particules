/*
  ==============================================================================

	VoiceManager.cpp
	Created: 6 Feb 2026 6:03:01pm
	Author:  kai

  ==============================================================================
*/

#include "VoiceManager.h"
#include "../framework/ParameterSnapshot.h"

VoiceManager::VoiceManager(GrainPool& p, PositionModulator& pm, EnvelopeLookUpTable& lut)
    : pool{p}, activeCount{0}, posMod{pm}, envLut{lut}
{
    reset();
}

void VoiceManager::reset()
{
    activeCount = 0;
    for(GrainHandle& handle : activeHandles)
        handle = GrainHandle::getInvalidState();
    pool.reset();
}

void VoiceManager::process(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource)
{
    //processGrainsSamples(outputBlock, bufferSize, inputSource);
    processSamplesGrains(outputBlock, bufferSize, inputSource);

    // TODO : proper AGC automatic gain compensation
    if(activeCount > 0)
    {
        const float scale = 1 / std::sqrt(static_cast<float>(activeCount));
        outputBlock.multiplyBy(scale);
    }

}

void VoiceManager::processGrainsSamples(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource)
{
    const size_t numChannels = outputBlock.getNumChannels();
    const int inputNumChannels = inputSource->getNumChannels();
    const int inputNumSamples = inputSource->getNumSamples();

    for(int i = activeCount - 1; i >= 0; --i) // backward iteration for removing handle securely
    {
        GrainHandle h = activeHandles[i];
        Grain* g = pool.get(h);
        //if(!g) // grain has been released already MAY NOT NEEDED
        //{
        //    removeVoice(i); 
        //    continue; 
        //}
        if(g->isExhausted())
        {
            pool.release(h);
            removeVoice(i);
        }

        for(int currentSample = 0; currentSample < bufferSize; ++currentSample)
        {
            const float phase = g->getPhase();
            for(int channel = 0; channel < numChannels; ++channel)
            {
                //const float* sample = inputSource->getReadPointer(channel % inputNumChannels);
                const float sampleValue = g->getCurrentSample(inputSource, channel, numChannels);
                const float envelopeValue = envLut.getEnvelopeValue(phase);
                outputBlock.addSample(channel, currentSample, sampleValue * envelopeValue);
            }
            g->update();
        }
    }
}

void VoiceManager::processSamplesGrains(AudioBlock& outputBlock, int bufferSize, const AudioBuffer* inputSource)
{
    const size_t numChannels = outputBlock.getNumChannels();
    //const int numSamples = outputBlock.getNumSamples();
    const int inputNumChannels = inputSource->getNumChannels();
    const int inputNumSamples = inputSource->getNumSamples();

    for(int currentSample = 0; currentSample < bufferSize; currentSample++)
    {
        for(int i = activeCount - 1; i >= 0; --i) // backward iteration for removing handle securely
        {
            GrainHandle h = activeHandles[i];
            Grain* g = pool.get(h);
            //if(!g) // grain has been released already MAY NOT NEEDED
            //{
            //    removeVoice(i); // place the current handle[index] in activeCount index and became
            //    continue; // restart the beginning of the loop at the same index
            //}

            const float phase = g->getPhase();
            for(int channel = 0; channel < numChannels; ++channel)
            {
                //const float* sample = inputSource->getReadPointer(channel % inputNumChannels);
                const float sampleValue = g->getCurrentSample(inputSource, channel, numChannels);
                const float envelopeValue = envLut.getEnvelopeValue(phase);
                outputBlock.addSample(channel, currentSample, sampleValue * envelopeValue);
            }
            g->update();
            if(g->isExhausted())
            {
                pool.release(h);
                removeVoice(i);
            }
        }
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

    envLut.setEnvelopeMode(snapshot.envMode);
    grain->config(snapshot, offset, posMod.computePhaseAtOffset(offset)); // init the grain here before process with the snapshot
    activeHandles[activeCount++] = handle;
}

// example : after spawning 5 times activeCount = 5
// removing index 2 then swapping the index 4 with the 2 and
// after decrementing activeCount is = 4.
void VoiceManager::removeVoice(const int i) { activeHandles[i] = activeHandles[--activeCount]; }
