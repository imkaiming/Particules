#pragma once
#include "../../framework/Core.h"

namespace particules
{
    class HandleSliderComponent : public juce::Slider
    {
    public:
        HandleSliderComponent()
        {
            setSliderStyle(juce::Slider::LinearHorizontal);
            setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
            setRange(0.0, 1.0);
            setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        }

        void paint(juce::Graphics& g) override
        {
            const float ratio = static_cast<float>((getValue() - getMinimum()) / (getMaximum() - getMinimum()));
            const float cx = ratio * static_cast<float>(getWidth());
            const float cy = static_cast<float>(getHeight()) * 0.5f;
            const float r = 6.0f;

            g.setColour(juce::Colours::white.withAlpha(0.9f));
            g.fillEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f);

            g.setColour(juce::Colours::black.withAlpha(0.4f));
            g.drawEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f, 1.0f);
        }

        float getNormalized() const { return static_cast<float>(getValue()); }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HandleSliderComponent)
    };
}