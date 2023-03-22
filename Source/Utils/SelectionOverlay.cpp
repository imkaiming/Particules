/*
  ==============================================================================

	SelectionOverlay.cpp
	Created: 21 Mar 2023 8:32:44pm
	Author:  user

  ==============================================================================
*/

#include "SelectionOverlay.h"


SelectionOverlay::SelectionOverlay()
{
	positionValue = 0.f;
	selectionValue = 1.f;
}

SelectionOverlay::~SelectionOverlay()
{
}

void SelectionOverlay::setPosition(float newValue)
{
	positionValue = newValue;
}

void SelectionOverlay::setSelection(float newValue)
{
	selectionValue = newValue;
	repaint();
}

void SelectionOverlay::paint(juce::Graphics& g)
{
	g.setColour(MyColours::red);
	g.fillRect(positionValue + 2.f, (float) getHeight() - 2.f, selectionValue, 3.f);
}

