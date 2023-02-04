/*
  ==============================================================================

	SpectrumComponent.cpp
	Created: 3 Feb 2023 10:19:39pm
	Author:  user

  ==============================================================================
*/

#include "SpectrumComponent.h"

SpectrumComponent::SpectrumComponent() {

}


SpectrumComponent::~SpectrumComponent() {
}


void SpectrumComponent::paint(juce::Graphics& g) {
	juce::Rectangle<float> spectrumComponent(150.0f, 1.f, 600.0f, 100.f);
	g.setColour(juce::Colours::aqua);
	g.fillRect(spectrumComponent);
	g.drawRect(spectrumComponent);


}

void SpectrumComponent::resized() {

}