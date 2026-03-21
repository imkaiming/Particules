#pragma once

#include "pch.h"

// global constant for the plugin

static constexpr int MAX_GRAINS = 500;
static constexpr int MAX_EVENTS = 16;
static constexpr double MAX_DURATION = 600.0; // 10 min
static constexpr int MAX_FILE_SIZE = 2 * 512ULL * 1024 * 1024; // 512MB limits ULL -> Unsigned Long Long
static constexpr float GRAIN_VISUAL_SIZE = 8.f;
static constexpr int LUT_SIZE = 2048;

//static constexpr int FFTSIZE_ORDER = 10;
//static constexpr int FFTSIZE = 1 << FFTSIZE_ORDER;
