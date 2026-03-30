#pragma once
#include "../../framework/Core.h"

namespace particules
{
    class HandleSliderLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        HandleSliderLookAndFeel() = default;

        ~HandleSliderLookAndFeel() = default;


        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
            float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider) override
        {
            const bool hover = slider.isMouseOverOrDragging();
            const float handleR = hover ? 8.f : 7.0f;

            const float cy = y + height * 0.5f;

            g.setColour(juce::Colours::white.withAlpha(0.3f));
            g.fillRect((float)x, cy - 1.0f, (float)width, 2.0f);

            g.setColour(juce::Colours::white);
            g.fillEllipse(sliderPos - handleR, cy - handleR, handleR * 2.0f, handleR * 2.0f);

            g.setColour(juce::Colours::black.withAlpha(0.4f));
            g.drawEllipse(sliderPos - handleR, cy - handleR, handleR * 2.0f, handleR * 2.0f, 1.0f);
        }
    };
}