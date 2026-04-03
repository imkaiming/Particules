#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

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

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HandleSliderLookAndFeel)
    };
}