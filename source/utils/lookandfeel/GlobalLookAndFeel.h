#pragma once

#include "../../framework/Core.h"
#include "BinaryData.h"

namespace particules
{
    class GlobalLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        GlobalLookAndFeel();
        ~GlobalLookAndFeel() = default ;

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle,
            float rotaryEndAngle, juce::Slider& slider) override;

        juce::Typeface::Ptr getFuturaTypeface() const noexcept { return futuraTypeface; }

    private:
        juce::Typeface::Ptr futuraTypeface;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalLookAndFeel)
    };

}
