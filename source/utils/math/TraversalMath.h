#pragma once

#include "MathConstants.h"
#include "framework/core/PluginParams.h"

namespace particules
{

    namespace gui
    {

        inline float evaluateTraversal(TraversalMode mode, float phase, float frequencyHz) noexcept
        {
            if(mode == TraversalMode::None)
                return 0.5f;

            const float p = std::fmod(phase * frequencyHz, 1.0f);
            float bipolarVal = 0.0f;

            switch(mode)
            {
                case TraversalMode::Sine:
                    bipolarVal = std::sin(twoPi * p);
                    break;

                case TraversalMode::Triangle:
                    bipolarVal = 2.0f * std::abs(2.0f * std::fmod(p + 0.75f, 1.0f) - 1.0f) - 1.0f;
                    break;

                case TraversalMode::Square:
                    bipolarVal = (p < 0.5f) ? 1.0f : -1.0f;
                    break;

                case TraversalMode::Random: // fake random
                    const float step = std::floor(phase * frequencyHz * 4.0f);
                    bipolarVal = std::fmod(std::sin(step * 12.9898f) * 43758.5453f, 2.0f) * 0.5f;
                    break;
            }

            return 0.5f + (bipolarVal * 0.5f);
        }
    }

}