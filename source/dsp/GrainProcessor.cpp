#include "GrainProcessor.h"

#include <juce_core/juce_core.h>

#include "GrainEnvelope.h"
#include "GrainPool.h"
#include "PositionModulator.h"
#include "utils/math/Lerp.h"
#include "utils/struct/ParameterSnapshot.h"
#include "utils/struct/SmoothedParameters.h"
#include "utils/struct/VisualSnapshot.h"

namespace particules
{
    GrainProcessor::GrainProcessor(GrainPool& p, PositionModulator& pm, GrainEnvelope& lut)
        : pool{p}, activeCount{0}, posMod{pm}, envLut{lut}
    {
        reset();
    }

    void GrainProcessor::reset() noexcept
    {
        activeCount = 0;
        for(GrainHandle& handle : activeHandles)
            handle = GrainHandle::getInvalidState();
        pool.reset();
    }

    void GrainProcessor::render(int currentSample, int outputNumChannels, float* const* outputPtrs, const float* const* inputPtrs,
        const SmoothedParameters& params, int numSamples)
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
            jassert(index >= 0 && index < numSamples);

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

    void GrainProcessor::spawn(const ParameterSnapshot& ps)
    {
        if(activeCount >= SIZE)
            return; // cannot spawn any more grains

        GrainHandle handle = pool.acquire();
        Grain* grain = pool.get(handle);
        if(grain == nullptr)
            return;

        visualY[handle.index] = juce::Random::getSystemRandom().nextFloat();

        envLut.setEnvelopeMode(ps.envMode);

        grain->config(ps, posMod.getPhase()); // init the grain here before process with the snapshot
        activeHandles[activeCount++] = handle;
    }

    // example : after spawning 5 times activeCount = 5
    // removing index 2 then swapping the index 4 with the 2 and
    // after decrementing activeCount is = 4.
    void GrainProcessor::removeVoice(const int i) { activeHandles[i] = activeHandles[--activeCount]; }

    void GrainProcessor::writeVisualSnapshot(VisualSnapshot& snap) noexcept
    {
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
    }
   
}