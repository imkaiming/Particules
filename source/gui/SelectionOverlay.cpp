/*
  ==============================================================================

	SelectionOverlay.cpp
	Created: 21 Mar 2023 8:32:44pm
	Author:  user

  ==============================================================================
*/

#include "SelectionOverlay.h"

SelectionOverlay::SelectionOverlay() : start{0.f}, width{1.f} { /*setSelection(0.f);*/ }

void SelectionOverlay::setPosition(float newValue)
{
    start = newValue;
    repaint();
}

void SelectionOverlay::setSelection(float newValue)
{
    width = newValue;
    repaint();
}

void SelectionOverlay::paint(juce::Graphics& g)
{
    g.setColour(MyColours::red);

    if(start + width > getWidth())
    {
        g.fillRect(start + 1.f, (float)getHeight() - 2.f, getWidth() - start, 4.f);
    }
    else
    {
        g.fillRect(start + 1.f, (float)getHeight() - 2.f, width, 4.f);
    }
}
