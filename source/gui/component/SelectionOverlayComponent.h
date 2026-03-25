#pragma once

#include "../../framework/Core.h"
#include "../../utils/MyColours.h"

namespace particules
{

    class SelectionOverlayComponent : public juce::Component
    {
    public:
        SelectionOverlayComponent();
        ~SelectionOverlayComponent() = default;

        void paint(juce::Graphics&) override;
        void setPosition(float);
        void setSelection(float);

    private:
        float start;
        float width;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectionOverlayComponent)
    };
}