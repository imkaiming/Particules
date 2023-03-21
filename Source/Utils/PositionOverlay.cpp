#include "PositionOverlay.h"

PositionOverlay::PositionOverlay()
{
	positionValue = 0;
}

PositionOverlay::~PositionOverlay()
{
}

void PositionOverlay::setPosition(float newValue)
{
	DBG(newValue);
	positionValue = newValue;
	repaint();
}

void PositionOverlay::paint(juce::Graphics& g)
{
	g.setColour(MyColours::cream);
	g.drawLine(positionValue, 0.f , positionValue + 1.0f, (float)getHeight(), 2.0f);
}

//void PositionOverlay::resized()
//{
//	repaint();
//}