#pragma once

#include <juce_core/juce_core.h>
//#include <juce_audio_processors/juce_audio_processors.h>

namespace juce
{
    class AudioProcessorValueTreeState;
}

namespace particules
{
    using ValueTreeState = juce::AudioProcessorValueTreeState; // juce::juce_audio_processors
    using str = juce::String; // juce::juce_core
}
