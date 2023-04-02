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
	repaint();
}

void SelectionOverlay::setSelection(float newValue)
{
	selectionValue = newValue;
	repaint();
}

void SelectionOverlay::paint(juce::Graphics& g)
{
	g.setColour(MyColours::red);

	if (positionValue + selectionValue > getWidth())
	{
		g.fillRect(positionValue + 1.f, (float)getHeight() - 2.f, getWidth() - positionValue, 4.f);
	}
	else
	{
		g.fillRect(positionValue + 1.f, (float)getHeight() - 2.f, selectionValue, 4.f);
	}
}


