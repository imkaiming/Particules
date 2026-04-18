#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/Core.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/enum/RotaryType.h"

namespace particules
{

    class AuxRotarySlider : public juce::Slider
    {
    public:
        AuxRotarySlider()
        {
            setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            setRange(0.0, 1.0);
            //getProperties().set("knobStyle", static_cast<int>(RotaryType::aux));
            setRepaintsOnMouseActivity(true);
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AuxRotarySlider)
    };
}