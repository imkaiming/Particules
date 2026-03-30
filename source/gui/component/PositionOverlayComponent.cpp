#include "PositionOverlayComponent.h"

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
        g.setColour(colours::cream.withAlpha(0.5f));
        g.drawLine(startPos, 0.f, startPos, (float)getHeight(), 2.0f);
    }

}