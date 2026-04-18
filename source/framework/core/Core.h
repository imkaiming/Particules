#pragma once

#include <cstdint>
#include <cmath> // std::floor 
#include <array>
#include <functional> 
#include <algorithm>  // std::clamp
#include <random>
#include <vector>
#include <span>
#include <cassert> // assert
#include <atomic>
#include <memory>
#include <utility> // std::max
#include <type_traits> // is_trivially_copyable


#if JUCE_DEBUG // require juce_core.h
    #define ENABLE_DEBUG_PRESET 0
    #define ENABLE_MELATONINE_INSPECTOR 0
#endif