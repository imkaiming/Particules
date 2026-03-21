#pragma once

#include "../../framework/Types.h"

class ParameterView;
class CustomLookAndFeel;
class ParticulesAudioProcessor;
class GrainVisualBuffer;
struct UIContext
{
    ValueTreeState& apvts;
    ParameterView& paramsView;
    CustomLookAndFeel& customLookAndFeel;
    ParticulesAudioProcessor& audioProcessor;
    GrainVisualBuffer& visualBuffer;
};