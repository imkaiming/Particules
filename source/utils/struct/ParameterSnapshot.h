#pragma once

#include "../enum/EnvelopeMode.h"
#include "../enum/TraversalMode.h"

namespace particules
{
    struct ParameterSnapshot
    {
        bool play;
        int durationSamples;
        int startPositionSamples;
        int selectionSamples;
        TraversalMode traversalMode;
        EnvelopeMode envMode;
        float emission;
        float linearGain;
        float speed;
        float sustainRatio;
        float traversalFreq;
        int inputNumSamples;
        bool isValid() const { return durationSamples > 0 && speed > 0 && emission >= 0; }
    };
}