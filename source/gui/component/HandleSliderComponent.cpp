#include "HandleSliderComponent.h"

#include <juce_graphics/juce_graphics.h>

namespace particules
{
    HandleSliderComponent::HandleSliderComponent()
    {
        setSliderStyle(juce::Slider::LinearHorizontal);
        setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        setRange(0.0, 1.0);
        setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
    }
    void HandleSliderComponent::paint(juce::Graphics& g)
    {
        const float ratio = static_cast<float>((getValue() - getMinimum()) / (getMaximum() - getMinimum()));
        const float cx = ratio * static_cast<float>(getWidth());
        const float cy = static_cast<float>(getHeight()) * 0.5f;
        const float r = 6.0f;

        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.fillEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f);

        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.drawEllipse(cx - r, cy - r, r * 2.0f, r * 2.0f, 1.0f);
    }

}