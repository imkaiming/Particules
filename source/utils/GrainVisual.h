/*
  ==============================================================================

    GrainPoint.h
    Created: 13 Apr 2023 8:50:17pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include "../framework/Core.h"

struct GrainVisual
{
    GrainVisual(float x, float y, float o) : opacity{o}, xPos{x}, yPos{y} {}
    GrainVisual() = default;
    ~GrainVisual() = default;

    float xPos; // the moving position of the grains playing
    float yPos; // a random value attributed when the grainPoint is created
    float opacity; // the volume directed by the envelope of the grains
};
