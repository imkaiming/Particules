/*
  ==============================================================================

    GrainVisualizer.h
    Created: 12 Apr 2023 1:47:43pm
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../Audio/Grain.h"
#include "StateParameters.h"

#pragma once

/**
* On a besoin de la valeur de la position du grain dans le buffer pour montrer sa position sur l'axe x.
* et on a besoin de la valeur de son relative à enveloppe pour la peindre sa transparence.
* Plus un son est fort plus il est opaque.
* Sur l'axe Y, les valeurs seront disposés aléatoirement.
* 
* La classe GrainVisualizer est appeler avec update à chaque lecture de sample ou à chaque chunk de buffer
*/

struct grainPoint
{
    float value;    // the volume directed by the envelope of the grains 
    int samplePos;  // the moving position of the grais playing
    int Ypos;       // a random value attributed when the grainPoint is created
};

class GrainVisualizer : public juce::Component
{
public:
    GrainVisualizer();// StateParameters* stateParams);// juce::Array<Grain*>* grains);
    ~GrainVisualizer() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void addGrainPoint();
    void update();
    void setGrains(juce::Array<Grain*>*);

private:
    // the number of grains in the scheduler is the same as the number of grainPoints.
    //StateParameters* stateParams;
    juce::Array<grainPoint> grainPoints;
    juce::Array<Grain*>* grains;
};