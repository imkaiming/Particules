#pragma once

#include "utils/enum/EnvelopeMode.h"
#include "utils/enum/TraversalMode.h"

namespace particules
{
    struct ParameterSnapshot
    {
        bool play;
        int durationSamples;
        int startPositionSamples;
        int spanSamples;
        TraversalMode traversalMode;
        EnvelopeMode envMode;
        float emission;
        float linearGain;
        float speed;
        float sustainRatio;
        float traversalFreq;
        int inputNumSamples;
        float playback;
        bool isValid() const { return durationSamples > 0 && speed > 0 && emission >= 0; }
    };
}