#include "VoiceManager.h"
#include "../utils/math/Lerp.h"
#include "../utils/struct/ParameterSnapshot.h"
#include "../utils/struct/SmoothedParameters.h"

namespace particules
{
    VoiceManager::VoiceManager(GrainPool& p, PositionModulator& pm, GrainEnvelope& lut, GrainVisualBuffer& vb)
        : pool{p}, activeCount{0}, posMod{pm}, envLut{lut}, visualBuffer{vb}
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

    void VoiceManager::render(int currentSample, int outputNumChannels, float* const* outputPtrs, const float* const* inputPtrs,
        const SmoothedParameters& params)
    {
        for(int i = activeCount - 1; i >= 0; --i) // backward iteration
        {
            GrainHandle h = activeHandles[i];
            Grain* g = pool.get(h);

            const float phase = g->getPhase();
            const float envelopeValue = envLut.getEnvelopeValue(phase);
            const float readPos = g->getReadPosition();

            int index = static_cast<int>(readPos);
            float frac = readPos - (float)index;

            for(int channel = 0; channel < outputNumChannels; ++channel)
            {
                // interpolating read position
                const float* sample = inputPtrs[channel];
                const float s0 = sample[index] * envelopeValue;
                const float s1 =
                    sample[index + 1] * envelopeValue; // buffer is safe because we added one value before setting the input

                outputPtrs[channel][currentSample] += lerp(s0, s1, frac);
                //outputBlock.addSample(channel, currentSample, lerp(s0, s1, frac) * envelopeValue);
            }

            g->nextReadPosition();
            g->updateParams(params);
            if(g->isExhausted())
            {
                pool.release(h);
                removeVoice(i);
            }
        }
    }

    void VoiceManager::spawn(const ParameterSnapshot& snapshot)
    {
        if(activeCount >= SIZE)
            return; // cannot spawn any more grains

        GrainHandle handle = pool.acquire();
        Grain* grain = pool.get(handle);
        if(grain == nullptr)
            return;

        visualY[handle.index] = juce::Random::getSystemRandom().nextFloat();

        envLut.setEnvelopeMode(snapshot.envMode);

        grain->config(snapshot, posMod.getPhase()); // init the grain here before process with the snapshot
        activeHandles[activeCount++] = handle;
    }

    // example : after spawning 5 times activeCount = 5
    // removing index 2 then swapping the index 4 with the 2 and
    // after decrementing activeCount is = 4.
    void VoiceManager::removeVoice(const int i) { activeHandles[i] = activeHandles[--activeCount]; }

    void VoiceManager::writeVisualSnapshot()
    {
        std::atomic<int>& index = visualBuffer.getReadIndex();
        const int write = 1 - index.load(std::memory_order_relaxed);
        auto& snap = visualBuffer.getSnapshot(write); // snapshot is not taken by GUI thread

        snap.count = 0;
        for(int i = 0; i < activeCount; ++i)
        {
            const GrainHandle h = activeHandles[i];
            const Grain* g = pool.get(h);
            if(g != nullptr) 
            {
                snap.grainVisuals[snap.count++] = {
                    g->getReadPosition(), visualY[h.index], envLut.getEnvelopeValue(g->getPhase())};
            }
        }

        //snap.count = activeCount;
        index.store(write, std::memory_order_release);
    }
}

/*

void VoiceManager::getAllActiveGrains(std::vector<GrainPoint>& out) const
{
    out.clear();
    out.reserve(activeCount);
    for(int i = 0; i < activeCount; ++i)
    {
        GrainHandle h = activeHandles[i];
        const Grain* g = pool.get(h);
        GrainPoint gp{g->getReadPosition(), visualY[h.index], envLut.getEnvelopeValue(g->getPhase())};
        out.push_back(gp);
    }
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
    const int inputNumChannels = inputSource->getNumChannels();
    const int inputNumSamples = inputSource->getNumSamples();

    for(int currentSample = 0; currentSample < bufferSize; currentSample++)
    {
        for(int i = activeCount - 1; i >= 0; --i) // backward iteration for removing handle securely
        {
            GrainHandle h = activeHandles[i];
            Grain* g = pool.get(h);

            const float phase = g->getPhase();
            for(int channel = 0; channel < numChannels; ++channel)
            {
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

*/