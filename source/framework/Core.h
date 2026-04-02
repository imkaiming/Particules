#pragma once

//#if BUILDING_PLUGIN_APP
//    #include "pch_plugin.h"
//#elif BUILDING_GUI_APP
//    #include "pch_gui.h"
//#else
//    #error "No build type defined for PCH"
//#endif

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

using ValueTreeState = juce::AudioProcessorValueTreeState;
using AudioBlock = juce::dsp::AudioBlock<float>;
using AudioBuffer = juce::AudioBuffer<float>;
using AudioLoadedCallback = std::function<void(AudioBuffer&)>;
using str = juce::String;


#include <cstdint>
#include <array>
#include <functional>
#include <random>
#include <span>
// only for compiler macros

#if JUCE_DEBUG
    #define ENABLE_DEBUG_PRESET 0
    #define ENABLE_MELATONINE_INSPECTOR 0
#endif