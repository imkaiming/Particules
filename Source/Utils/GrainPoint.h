/*
  ==============================================================================

    GrainPoint.h
    Created: 13 Apr 2023 8:50:17pm
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once


class GrainPoint
{
public:
	GrainPoint();
	~GrainPoint();

	void setOpacity(float);
	void setSamplePos(float);

	float getOpacity();
	float getSamplePos();
	float getYpos();


private:
	float opacity;    // the volume directed by the envelope of the grains 
	int samplePos;  // the moving position of the grais playing
	int Ypos;       // a random value attributed when the grainPoint is created
};