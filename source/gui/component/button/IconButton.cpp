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
        auto bounds = getLocalBounds().toFloat();

        float alpha = isPressed ? 0.8f : (isHovered ? 1.0f : 0.6f);

        g.setColour(juce::Colours::black.withAlpha(0.2f));
        g.fillRoundedRectangle(bounds, 6.0f);

        if(currentIcon != nullptr)
        {
            currentIcon->drawWithin(g, bounds.reduced(padding), juce::RectanglePlacement::centred, alpha);
        }
    }

}