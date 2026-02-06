/*
  ==============================================================================

    Core.h
    Created: 5 Feb 2026 9:54:13pm
    Author:  kai

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Lib/AudioFFT.h"
#include "../Utils/ParamsID.h"

using ValueTreeState = juce::AudioProcessorValueTreeState;
using AudioBlock = juce::dsp::AudioBlock<float>;
using Buffer = juce::AudioBuffer<float>;

// TODO@ separer la definition dans le cpp et la declaration
inline float convertToPercentage(float toConvert)
{
    return juce::jlimit(0.0f, 1.0f, toConvert * 0.01f);
}