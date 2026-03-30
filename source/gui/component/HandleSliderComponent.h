#pragma once
#include "../../framework/Core.h"

namespace particules
{
    class HandleSliderComponent : public juce::Slider
    {
    public:
        HandleSliderComponent();

        void paint(juce::Graphics&) override;

        float getNormalized() const { return static_cast<float>(getValue()); }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HandleSliderComponent)
    };
}