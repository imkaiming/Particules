/*
  ==============================================================================

	GrainPoint.cpp
	Created: 13 Apr 2023 8:50:17pm
	Author:  user

  ==============================================================================
*/

#include "GrainPoint.h"

GrainPoint::GrainPoint()
{
	juce::Random r;
	Ypos = r.nextFloat();

	opacity = 0.0f;
	samplePos = 0;

}

GrainPoint::~GrainPoint()
{
}


void GrainPoint::setOpacity(float newValue)
{
	opacity = newValue;
}

void GrainPoint::setSamplePos(float newValue)
{
	samplePos = newValue;
}

float GrainPoint::getOpacity()
{
	return opacity;
}

float GrainPoint::getSamplePos() {
	return samplePos;
}

float GrainPoint::getYpos() {
	return Ypos;
}