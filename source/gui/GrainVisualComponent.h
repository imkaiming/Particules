
/*
  ==============================================================================

	GrainVisualizer.h
	Created: 12 Apr 2023 1:47:43pm
	Author:  user

  ==============================================================================
*/



#include "../utils/MyColours.h"
#include "../framework/ParamsID.h"
#include "../framework/GrainVisualBuffer.h"
#include "../framework/Core.h"

#pragma once

/**
* On a besoin de la valeur de la position du grain dans le buffer pour montrer sa position sur l'axe x.
* et on a besoin de la valeur de son relative à enveloppe pour la peindre sa transparence.
* Plus un son est fort plus il est opaque.
* Sur l'axe Y, les valeurs seront disposés aléatoirement.
*
* La classe GrainVisualComponent est appelé avec update à chaque lecture de sample ou à chaque chunk de buffer
*/


class GrainVisualComponent : public juce::Component, private juce::Timer
{
public:
    GrainVisualComponent(GrainVisualBuffer& vb);
    ~GrainVisualComponent() override;

	void paint(juce::Graphics& g) override;
	void resized() override;

	void setNumSamples(const float);

private:
    static constexpr int MAXGRAINS = static_cast<int>(Param::MaxGrains);
    static constexpr float GSIZE = Param::GrainVisual::Size;
    static constexpr float GCENTER = GSIZE / 2.f;
	void timerCallback() override;

	juce::Colour colour;
    GrainVisualBuffer& visualBuffer;
	float numSamples;

	float invWidthSamples;
};