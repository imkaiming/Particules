#pragma once

#include "pch.h"

// global constant for the plugin

namespace particules
{
    inline constexpr int maxMidiVoice = 16;
    inline constexpr int maxSpawnsPerBlock = 16;
    inline constexpr double maxFileDuration = 600.0; // 10 min
    inline constexpr int maxFileSize = 2 * 512ULL * 1024 * 1024; // 512MB limits ULL -> Unsigned Long Long
    inline constexpr int maxLutSize = 2048;

    namespace Grains
    {
        inline constexpr int maxGrains = 500;
        inline constexpr float maxDuration = 10.0f; // seconds
    }

    namespace UI
    {
        inline constexpr float grainVisualSize = 8.f;
    }
}

//static constexpr int FFTSIZE_ORDER = 10;
//static constexpr int FFTSIZE = 1 << FFTSIZE_ORDER;
