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

    void PhaseGenerator::setIncrement() { increment = frequency / (float)sampleRate; }

    void PhaseGenerator::advance(int numSamples)
    {
        phase += increment * numSamples;

        // truncation wrap
        if(phase >= 1.f)
            phase -= static_cast<int>(phase); // std::floor ?
    }

    float PhaseGenerator::getPhase() const noexcept { return phase; }

}