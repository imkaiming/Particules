#pragma once
#include "../button/EnvelopeButtonMenu.h"
#include "ButtonSliderControlGroup.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    class EnvelopeControlGroup : public ButtonSliderControlGroup
    {
    public:
        EnvelopeControlGroup(UIContext& uic);

    private:
        juce::Component& getButton() override { return buttonMenu; }

        EnvelopeButtonMenu buttonMenu;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeControlGroup)
    };
}