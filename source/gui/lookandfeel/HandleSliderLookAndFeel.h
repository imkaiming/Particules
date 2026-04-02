#pragma once
#include "../../framework/Core.h"

namespace particules
{
    class HandleSliderLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        HandleSliderLookAndFeel();

        ~HandleSliderLookAndFeel() = default;

        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
            float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider) override;

    private:
        juce::Image metalHandleImage;
    };
}