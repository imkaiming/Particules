#pragma once

#include <juce_core/juce_core.h>

#include "framework/core/Core.h"

namespace particules
{
    inline std::unique_ptr<juce::AudioParameterFloat> createNormalizedParameter(const juce::ParameterID& id, const str& name,
        float min, float max, float skew, float init, std::function<str(float, int)> stringFromValueFunc,
        std::function<float(const str&)> valueFromStringFunc)
    {
        juce::NormalisableRange<float> range{min, max};
        range.setSkewForCentre(skew);

        return std::make_unique<juce::AudioParameterFloat>(id, name, range, init,
            juce::AudioParameterFloatAttributes{}
                .withCategory(juce::AudioProcessorParameter::genericParameter)
                .withStringFromValueFunction(stringFromValueFunc)
                .withValueFromStringFunction(valueFromStringFunc));
    }

    inline std::unique_ptr<juce::AudioParameterFloat> createNormalizedParameter(
        const juce::ParameterID& id, const str& name, float min, float max, float skew = 0.5f, float init = 0.5f)
    {
        juce::NormalisableRange<float> range{min, max};
        range.setSkewForCentre(skew);

        return std::make_unique<juce::AudioParameterFloat>(id, name, range, init);
    }
}