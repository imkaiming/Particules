#pragma once

#include "framework/core/PluginParams.h"

// return normalized phase value based on the freq / sampleRate

namespace particules
{
    class PhaseGenerator
    {
    public:
        PhaseGenerator() = default;
        ~PhaseGenerator() = default;

        void setSampleRate(double sr);
        void setFrequency(float f);
        void reset() { phase = 0; }
        void advance(int numSamples);

        float getPhase() const noexcept;

    private:
        static constexpr int SIZE = params::maxLutSize;

        void setIncrement() ;
        double sampleRate = 0.0;
        float phase = 0.f;
        float frequency = 0.f;
        float increment = 0.f;
    };
}