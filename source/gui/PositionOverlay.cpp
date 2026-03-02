#include "PositionOverlay.h"

PositionOverlay::PositionOverlay() : startPos{0.f} { /* setPosition(0.f);*/ }

float PositionOverlay::getPosition() { return startPos; }

void PositionOverlay::setPosition(float newValue)
{
    startPos = newValue;
    repaint();
}

void PositionOverlay::paint(juce::Graphics& g)
{
    g.setColour(MyColours::cream);
    g.drawLine(startPos, 0.f, startPos, (float)getHeight(), 2.0f);
}
