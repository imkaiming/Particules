#pragma once

#include <juce_core/juce_core.h> // end macro
#include <juce_gui_basics/juce_gui_basics.h> // component

namespace juce
{
    class AudioThumbnail;
    class Graphics;
}
class VignetteOverlayComponent : public juce::Component
{
public:
    VignetteOverlayComponent() = default;
    ~VignetteOverlayComponent() = default;

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        const float w = (float)bounds.getWidth();
        const float h = (float)bounds.getHeight();

        const float alpha = 0.16f;

        juce::Graphics::ScopedSaveState save(g);

        juce::ColourGradient leftGrad(
            juce::Colours::black.withAlpha(alpha), 0, h * 0.5f, juce::Colours::transparentBlack, w * 0.25f, h * 0.5f, false);
        g.setGradientFill(leftGrad);
        g.fillRect(0.f, 0.f, w * 0.4f, h);

        juce::ColourGradient rightGrad(
            juce::Colours::transparentBlack, w * 0.75f, h * 0.5f, juce::Colours::black.withAlpha(alpha), w, h * 0.5f, false);
        g.setGradientFill(rightGrad);
        g.fillRect(w * 0.6f, 0.f, w * 0.4f, h);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VignetteOverlayComponent)
};