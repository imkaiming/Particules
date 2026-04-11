#include "DrawButtonMenu.h"

namespace particules
{
    DrawButtonMenu::DrawButtonMenu() { setRepaintsOnMouseActivity(true); }

    void DrawButtonMenu::resized() { buttonBounds = getLocalBounds().toFloat(); }

    void DrawButtonMenu::paint(juce::Graphics& g)
    {
        const bool isHover = isMouseOver();
        const float w = juce::jmin(buttonBounds.getWidth(), buttonBounds.getHeight());
        auto drawArea = buttonBounds.reduced(w / 4.f);

        g.setColour(isHover ? juce::Colour(0xff1e1e1e).brighter(0.05f) : juce::Colour(0xff1e1e1e));
        g.fillRoundedRectangle(buttonBounds, 5.0f);

        g.setColour(isHover ? juce::Colours::white.withAlpha(0.5f) : coloursv2::midGrey);
        g.drawRoundedRectangle(buttonBounds, 10.0f, 1.0f);

        juce::Path curve = createCurvePath(drawArea);

        g.setColour(curveColour);
        g.strokePath(curve, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
    }

    juce::Path DrawButtonMenu::buildPathFromFunction(
        juce::Rectangle<float> bounds, std::function<float(float)> evalFunc, int numPoints)
    {
        juce::Path p;
        const float padding = 4.0f;
        const float drawWidth = bounds.getWidth() - (padding * 2.0f);
        const float drawHeight = bounds.getHeight() - (padding * 2.0f);

        for(int i = 0; i <= numPoints; ++i)
        {
            float phase = static_cast<float>(i) / numPoints;
            float val = evalFunc(phase);

            float x = bounds.getX() + padding + (phase * drawWidth);
            float y = bounds.getY() + padding + drawHeight * (1.0f - val);

            if(i == 0)
                p.startNewSubPath(x, y);
            else
                p.lineTo(x, y);
        }

        return p;
    }
}