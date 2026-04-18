#pragma once
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/Core.h"
#include "framework/core/GuiTypes.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    class IconButton : public juce::Button
    {
    public:
        IconButton(const str&);

        void setIcon(juce::Drawable* newIcon);
        void paintButton(juce::Graphics& g, bool isHovered, bool isPressed) override;

        float internalPaddingCoef;
    private:
        juce::Drawable* currentIcon = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconButton)
    };
}