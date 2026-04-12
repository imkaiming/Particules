#include "IconButton.h"

#include "../../lookandfeelv2/Colours.h"
#include <juce_graphics/juce_graphics.h>

namespace particules
{

    IconButton::IconButton(const str& name) : Button(name) { setClickingTogglesState(false); }

    void IconButton::setIcon(juce::Drawable* newIcon)
    {
        if(currentIcon != newIcon)
        {
            currentIcon = newIcon;
            repaint();
        }
    }

    void IconButton::paintButton(juce::Graphics& g, bool isHovered, bool isPressed)
    {
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();

        const float alpha = isPressed ? 0.8f : (isHovered ? 1.0f : 0.6f);
        const float padding = getWidth() * 0.25f;

        g.setColour(coloursv2::lightBlack);
        g.fillRoundedRectangle(bounds, 6.0f);
        g.setColour(coloursv2::blackest);
        g.drawRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 12.0f, getWidth() * 0.05f);

        if(currentIcon != nullptr)
        {
            currentIcon->drawWithin(g, bounds.reduced(padding), juce::RectanglePlacement::centred, alpha);
        }
    }

}