#pragma once

#include <juce_core/juce_core.h>
//#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../lookandfeelv2/Colours.h"
#include "../../../utils/enum/RotaryType.h"

namespace particules
{

    class PrimaryRotarySlider : public juce::Slider
    {
    public:
        PrimaryRotarySlider()
        {
            setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

            setColour(juce::Slider::rotarySliderFillColourId, colours::violetBleu);
            setColour(juce::Slider::rotarySliderOutlineColourId, colours::grisAnthracite);

            getProperties().set("knobStyle", static_cast<int>(RotaryType::primary));
            setRepaintsOnMouseActivity(true);

        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrimaryRotarySlider)
    };

    class SecondaryRotarySlider : public juce::Slider
    {
    public:
        SecondaryRotarySlider()
        {
            setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

            setColour(juce::Slider::rotarySliderFillColourId, colours::violetBleu);
            setColour(juce::Slider::rotarySliderOutlineColourId, colours::perleBlanc);

            getProperties().set("knobStyle", static_cast<int>(RotaryType::secondary));
            setRepaintsOnMouseActivity(true);

        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SecondaryRotarySlider)
    };

    class TertiaryRotarySlider : public juce::Slider
    {
    public:
        TertiaryRotarySlider()
        {
            setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            getProperties().set("knobStyle", static_cast<int>(RotaryType::tertiary));
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TertiaryRotarySlider)
    };
}