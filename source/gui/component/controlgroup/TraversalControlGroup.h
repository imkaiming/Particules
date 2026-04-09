#pragma once
#include "../button/TraversalButtonMenu.h"
#include "ButtonSliderControlGroup.h"

namespace particules
{
    class TraversalControlGroup : public ButtonSliderControlGroup
    {
    public:
        TraversalControlGroup(UIContext& uic);

    private:
        juce::Component& getButton() override { return buttonMenu; }

        TraversalButtonMenu buttonMenu;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TraversalControlGroup)
    };
}