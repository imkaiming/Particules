#pragma once

#include "pch.h"

#include <cstdint>
#include <array>
#include <functional>
#include <random>
#include <span>
// only for compiler macros

#if JUCE_DEBUG
    #define ENABLE_DEBUG_PRESET 0
    #define ENABLE_MELATONINE_INSPECTOR 1
#endif