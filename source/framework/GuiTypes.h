#pragma once

#include <juce_core/juce_core.h>
#include "Core.h"

namespace juce
{
    class AudioProcessorValueTreeState;
    class Colour;
}

namespace particules
{
    using ValueCallback = std::function<void(double)>;
    using ValueTreeState = juce::AudioProcessorValueTreeState; // juce::juce_audio_processors
    using str = juce::String; // juce::juce_core
    using color = juce::Colour;
}
