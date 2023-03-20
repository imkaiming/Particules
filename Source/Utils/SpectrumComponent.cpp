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
	//juce::Rectangle<float> spectrumComponent(5.0f, 5.f, 850.0f, 138.f);
	//g.setColour(juce::Colours::black);
	//g.fillRect(spectrumComponent);
	//g.drawRect(spectrumComponent);
	g.fillAll(MyColours::black);

}

void SpectrumComponent::resized() {

}