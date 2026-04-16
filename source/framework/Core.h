#pragma once

//#if BUILDING_PLUGIN_APP
//    #include "pch_plugin.h"
//#elif BUILDING_GUI_APP
//    #include "pch_gui.h"
//#else
//    #error "No build type defined for PCH"
//#endif

//#include <juce_audio_basics/juce_audio_basics.h>
//#include <juce_audio_formats/juce_audio_formats.h>
//#include <juce_audio_processors/juce_audio_processors.h>
//#include <juce_audio_utils/juce_audio_utils.h>
//#include <juce_core/juce_core.h>
//#include <juce_dsp/juce_dsp.h>
//#include <juce_events/juce_events.h>
//#include <juce_graphics/juce_graphics.h>
//#include <juce_gui_basics/juce_gui_basics.h>


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
#include <utility> // std::max
#include <type_traits> // is_trivially_copyable


#if JUCE_DEBUG
    #define ENABLE_DEBUG_PRESET 0
    #define ENABLE_MELATONINE_INSPECTOR 0
#endif