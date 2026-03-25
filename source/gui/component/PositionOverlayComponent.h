
#pragma once
#include "../../framework/Core.h"
#include "../../utils/MyColours.h"

namespace particules
{

    class PositionOverlayComponent : public juce::Component
    {
    public:
        PositionOverlayComponent();
        ~PositionOverlayComponent() = default;

        void paint(juce::Graphics&) override;
        void setPosition(float);
        float getPosition();

    private:
        float startPos;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionOverlayComponent)
    };
}