#pragma once

#include "../../framework/Constants.h"
#include "GrainVisual.h"

namespace particules
{
    struct VisualSnapshot
    {
        static constexpr int SIZE = global::maxGrains;

        std::array<GrainVisual, SIZE> grainVisuals{};
        int count = 0;
    };
}