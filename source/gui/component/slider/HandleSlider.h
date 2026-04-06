#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace juce
{
    class Graphics;
}

namespace particules
{
    class HandleSlider : public juce::Slider
    {
    public:
        HandleSlider();

        void paint(juce::Graphics&) override;

        float getNormalized() const { return static_cast<float>(getValue()); }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HandleSlider)
    };
}