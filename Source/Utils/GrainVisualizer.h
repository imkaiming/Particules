
/*
  ==============================================================================

	GrainVisualizer.h
	Created: 12 Apr 2023 1:47:43pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../Audio/Grain.h"
#include "MyColours.h"
#include "../Framework/ParamsID.h"

#pragma once

/**
* On a besoin de la valeur de la position du grain dans le buffer pour montrer sa position sur l'axe x.
* et on a besoin de la valeur de son relative à enveloppe pour la peindre sa transparence.
* Plus un son est fort plus il est opaque.
* Sur l'axe Y, les valeurs seront disposés aléatoirement.
*
* La classe GrainVisualizer est appeler avec update à chaque lecture de sample ou à chaque chunk de buffer
*/


class GrainVisualizer : public juce::Component, private juce::Timer
{
public:
	GrainVisualizer(juce::Array<Grain*>* grains);
	~GrainVisualizer() override;

	void paint(juce::Graphics& g) override;
	void resized() override;

	void setNumSamples(int);
	void setGrains(juce::Array<Grain*>* grains);

private:

	void timerCallback() override;

	juce::Colour colour;
	juce::Array<Grain*>* grains;
	int numSamples;
};