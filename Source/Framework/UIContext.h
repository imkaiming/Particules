/*
  ==============================================================================

    UIContext.h
    Created: 8 Feb 2026 5:03:17pm
    Author:  kai

  ==============================================================================
*/

#pragma once

#include "Core.h"

class ParameterView;
class CustomLookAndFeel;
class ParticulesAudioProcessor;
struct UIContext
{
    ValueTreeState& apvts;
    ParameterView& paramsView;
    CustomLookAndFeel& customLookAndFeel;
    ParticulesAudioProcessor& audioProcessor;
};