#pragma once

#include "framework/core/PluginParams.h"
#include "GrainVisual.h"

namespace particules
{
    struct VisualSnapshot
    {
        static constexpr int SIZE = params::maxActiveGrains;

        std::array<GrainVisual, SIZE> grainVisuals{};
        int count = 0;
    };
}