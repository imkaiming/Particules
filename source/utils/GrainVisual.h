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
    GrainVisual(float xPos, float yPos, float opacity) : opacity{opacity}, xPos{xPos}, yPos{yPos} {}
    GrainVisual() = default;
    ~GrainVisual() = default;

    float xPos; // the moving position of the grains playing
    float yPos; // a random value attributed when the grainPoint is created
    float opacity; // the volume directed by the envelope of the grains
};
