#pragma once

#include "../../framework/Core.h"
#include "../../utils/MyColours.h"

namespace particules
{

    class SpanOverlayComponent : public juce::Component
    {
    public:
        SpanOverlayComponent();
        ~SpanOverlayComponent() = default;

        void paint(juce::Graphics&) override;
        void setPosition(float);
        void setSpan(float);

    private:
        float start;
        float width;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpanOverlayComponent)
    };
}