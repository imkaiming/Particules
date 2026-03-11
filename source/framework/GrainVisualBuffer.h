#pragma once

#include "../utils/GrainPoint.h"
#include "ParamsID.h"
// snapshot of every actives grains published every buffers end

class GrainVisualBuffer
{
public:
    GrainVisualBuffer() = default;
    ~GrainVisualBuffer() = default;

private:
    static constexpr int SIZE = Param::MaxGrains;

    std::array<GrainPoint, SIZE> grainPoints;
    std::atomic<int> count{0};
};