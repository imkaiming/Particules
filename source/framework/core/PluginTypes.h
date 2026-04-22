#pragma once

// type aliases used in the project

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

#include <functional>

namespace particules
{
    struct AudioPayload;
    struct ParameterSnapshot;

    using ValueTreeState = juce::AudioProcessorValueTreeState; // juce::juce_audio_processors
    using AudioBlock = juce::dsp::AudioBlock<float>; // juce::juce_dsp
    using AudioBuffer = juce::AudioBuffer<float>; // juce::juce_audio_basics
    using AudioLoadedCallback = std::function<void(std::unique_ptr<AudioBuffer>, const juce::File&)>; // juce::juce_audio_basics
    using str = juce::String; // juce::juce_core
    using SpawnGrainCallback = std::function<void(const ParameterSnapshot&, AudioPayload*, int, float, float)>;
}
