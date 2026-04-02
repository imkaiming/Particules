#pragma once

// global constant for the plugin

namespace particules
{

    inline constexpr int maxMidiVoice = 16;
    inline constexpr int maxSpawnsPerBlock = 16;
    inline constexpr double maxFileDuration = 600.0; // 10 min
    inline constexpr int maxFileSize = 2 * 512ULL * 1024 * 1024; // 512MB limits ULL -> Unsigned Long Long
    inline constexpr int maxLutSize = 2048;

    namespace global
    {
        inline constexpr int maxGrains = 500;
        inline constexpr float maxDuration = 10.0f; // seconds
    }

    namespace UI
    {
        inline constexpr const float grainVisualSize = 8.f;

        inline constexpr const int windowWidthInit = 750; 
        inline constexpr const int windowWidthMin = 600; 
        inline constexpr const int windowWidthMax = 900; 

        inline constexpr const int windowHeightInit = 500; 
        inline constexpr const int windowHeightMin = 400; 
        inline constexpr const int windowHeightMax = 600; 
    }
}

//static constexpr int FFTSIZE_ORDER = 10;
//static constexpr int FFTSIZE = 1 << FFTSIZE_ORDER;
