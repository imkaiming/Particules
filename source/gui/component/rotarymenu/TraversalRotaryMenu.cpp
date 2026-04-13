#include "TraversalRotaryMenu.h"
#include "../../../utils/math/TraversalMath.h"

namespace particules
{
    TraversalRotaryMenu::TraversalRotaryMenu(ValueTreeState& apvts, const str& mId, const str& fId, const str& jId)
        : RotaryMenuBase(apvts, mId, fId, jId, "Traversal", "traversal")
    {
    }

    juce::Image TraversalRotaryMenu::createMenuIcon(int itemIndex)
    {
        const int w = 200;
        const int h = 40;
        juce::Image img(juce::Image::ARGB, w, h, true);
        juce::Graphics g(img);
        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

        auto mode = static_cast<TraversalMode>(itemIndex);
        juce::Path p;
        const float padding = 5.0f;
        const float drawW = w - (padding * 2.0f);
        const float drawH = h - (padding * 2.0f);

        for(int i = 0; i <= w; ++i)
        {
            float phase = static_cast<float>(i) / w;
            float val = gui::evaluateTraversal(mode, phase, 0.5f);

            float x = padding + (phase * drawW);
            float y = padding + drawH * (1.0f - val);

            if(i == 0)
                p.startNewSubPath(x, y);
            else
                p.lineTo(x, y);
        }

        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(2.5f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));

        return img;
    }
}