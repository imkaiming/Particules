/*
  ==============================================================================

    GrainPoint.h
    Created: 13 Apr 2023 8:50:17pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include "../framework/Core.h"

class GrainPoint
{
public:
    GrainPoint() : opacity{0.f}, samplePos{0}, yPos{0}
    {
        juce::Random r;
        yPos = static_cast<int>(r.nextFloat());
    }

    ~GrainPoint() = default;

    float opacity; // the volume directed by the envelope of the grains
    int samplePos; // the moving position of the grais playing
    int yPos; // a random value attributed when the grainPoint is created

};
