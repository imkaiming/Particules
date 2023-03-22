#include "PositionOverlay.h"

PositionOverlay::PositionOverlay()
{
	positionValue = 0.f;
}

PositionOverlay::~PositionOverlay()
{
}

float PositionOverlay::getPosition()
{
	return positionValue;
}



void PositionOverlay::setPosition(float newValue)
{
	positionValue = newValue;
	repaint();
}

void PositionOverlay::paint(juce::Graphics& g)
{
	g.setColour(MyColours::cream);
	g.drawLine(positionValue, 0.f, positionValue, (float)getHeight(), 2.0f);
}

//void PositionOverlay::resized()
//{
//	repaint();
//}