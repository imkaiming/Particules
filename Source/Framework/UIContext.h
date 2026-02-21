/*
  ==============================================================================

    UIContext.h
    Created: 8 Feb 2026 5:03:17pm
    Author:  kai

  ==============================================================================
*/

#pragma once

#include "../Framework/Core.h"

//using ValueTreeState = juce::AudioProcessorValueTreeState;
class ParameterView;
class CustomLookAndFeel;
struct UIContext
{
    ValueTreeState& apvts;
    ParameterView& paramsView;
    CustomLookAndFeel& customLookAndFeel;
};