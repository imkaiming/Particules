/*
  ==============================================================================

    GrainPoint.h
    Created: 13 Apr 2023 8:50:17pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>

class GrainPoint
{
public:
	GrainPoint();
	~GrainPoint();

	void setOpacity(float);
	void setSamplePos(int);

	float getOpacity();
	int getSamplePos();
	int getYpos();


private:
	float opacity;    // the volume directed by the envelope of the grains 
	int samplePos;  // the moving position of the grais playing
	int Ypos;       // a random value attributed when the grainPoint is created
};