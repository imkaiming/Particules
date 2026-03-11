#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>


#include <atomic>
#include <cstdint>
#include <functional>


//#define GRAINPOINT_SIZE 10.f
#define FFTSIZE_ORDER 10
#define FFTSIZE static_cast<int>(std::pow(2, FFTSIZE_ORDER))
//#define MAX_DURATION (120)   // le nombre de second maximum pour loader un fichier audio
//#define MAX_GRAINS 500       //
//#define MAX_EVENT 16         // preallocation for the std::array<int> event used in the scheduler

using ValueTreeState = juce::AudioProcessorValueTreeState;
using AudioBlock = juce::dsp::AudioBlock<float>;
using AudioBuffer = juce::AudioBuffer<float>;
using str = juce::String;

namespace Param
{
    constexpr int MaxGrains = 500;
    constexpr int MaxEvents = 16;
    constexpr int MaxDuration = 120;
    constexpr int MaxFileSize = 512ULL * 1024 * 1024; // 512MB limits ULL -> Unsigned Long Long
}

static constexpr const float pi = juce::MathConstants<float>().pi;
static constexpr const float twoPi = juce::MathConstants<float>().twoPi;
static constexpr const float halfPi = juce::MathConstants<float>().halfPi;

// TODO@ separer la definition dans le cpp et la declaration
inline float convertToPercentage(float toConvert) noexcept
{
    return juce::jlimit(0.0f, 1.0f, toConvert * 0.01f); // 0 .. 100
}

inline float lerp(float a, float b, float f) { return a * (1.0f - f) + (b * f); };

#if JUCE_DEBUG
    #define ENABLE_DEBUG_PRESET 1
#endif