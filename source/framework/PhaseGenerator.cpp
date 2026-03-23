#include "PhaseGenerator.h"

namespace particules
{
    void PhaseGenerator::setSampleRate(double sr)
    {
        jassert(sr > 0);
        sampleRate = static_cast<float>(sr);
    }
    void PhaseGenerator::setFrequency(float f)
    {
        frequency = std::max(f, 0.001f);
        if(sampleRate > 0)
            setIncrement();
    }

    void PhaseGenerator::setIncrement() { increment = frequency / static_cast<float>(sampleRate); }

    void PhaseGenerator::advance(int numSamples)
    {
        phase += increment * numSamples;
        phase -= std::floor(phase);
    }

    float PhaseGenerator::getPhase() const noexcept
    {
        float p = phase;
        p -= std::floor(p);
        return p;
    }
}