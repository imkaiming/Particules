#pragma once

#include "pch.h"

// type aliases used in the project

using ValueTreeState = juce::AudioProcessorValueTreeState;
using AudioBlock = juce::dsp::AudioBlock<float>;
using AudioBuffer = juce::AudioBuffer<float>;
using str = juce::String;
using AudioLoadedCallback = std::function<void(AudioBuffer&)>;