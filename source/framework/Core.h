#pragma once

#include "pch.h"

#include <cstdint>
#include <array>
#include <functional>
#include <random>
// only for compiler macros

#if JUCE_DEBUG
    #define ENABLE_DEBUG_PRESET 1
    #define ENABLE_MELATONINE_INSPECTOR 0
#endif