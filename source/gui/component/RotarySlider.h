#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../lookandfeelv2/Colours.h"

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

            getProperties().set("knobStyle", "primary");
        }

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

            getProperties().set("knobStyle", "secondary");
        }
    };
}