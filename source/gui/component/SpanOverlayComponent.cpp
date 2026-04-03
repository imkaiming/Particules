#include "SpanOverlayComponent.h"

#include <juce_graphics/juce_graphics.h>

//#include "../../framework/Core.h"
#include "../lookandfeel/MyColours.h"

namespace particules
{

    SpanOverlayComponent::SpanOverlayComponent() : start{0.f}, width{0.f} {}

    void SpanOverlayComponent::setPosition(float newValue)
    {
        start = newValue;
        repaint();
    }

    void SpanOverlayComponent::setSpan(float newValue)
    {
        width = newValue;
        repaint();
    }

    void SpanOverlayComponent::paint(juce::Graphics& g)
    {
        g.setColour(colours::red.withAlpha(0.5f));

        const float totalWidth = static_cast<float>(getWidth());
        float x = start;
        float w = width;

        if(x + w > totalWidth)
            w = totalWidth - x;

        if(w > 0.0f && x < totalWidth)
            g.fillRect(x, 0.0f, w, static_cast<float>(getHeight()));
    }

}