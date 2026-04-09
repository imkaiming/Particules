#include "PositionOverlayComponent.h"

#include <juce_graphics/juce_graphics.h>

#include "../../lookandfeelv2/Colours.h"

namespace particules
{

    PositionOverlayComponent::PositionOverlayComponent() : startPos{0.f} { /* setPosition(0.f);*/ }

    float PositionOverlayComponent::getPosition() { return startPos; }

    void PositionOverlayComponent::setPosition(float newValue)
    {
        startPos = newValue;
        repaint();
    }

    void PositionOverlayComponent::paint(juce::Graphics& g)
    {
        g.setColour(coloursv2::perleBlanc.withAlpha(0.5f));
        g.drawLine(startPos, 0.f, startPos, static_cast<float>(getHeight()), 2.0f);
    }

}