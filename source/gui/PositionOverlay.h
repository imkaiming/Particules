
#pragma once
#include "../framework/Core.h"
#include "../utils/MyColours.h"

namespace particules
{

    class PositionOverlay : public juce::Component
    {
    public:
        PositionOverlay();
        ~PositionOverlay() = default;

        void paint(juce::Graphics&) override;
        void setPosition(float);
        float getPosition();

    private:
        float startPos;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionOverlay)
    };
}