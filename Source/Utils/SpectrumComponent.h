/*
  ==============================================================================

	SpectrumComponent.h
	Created: 3 Feb 2023 10:19:39pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

class SpectrumComponent : public juce::Component {
public:
	SpectrumComponent();
	~SpectrumComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumComponent)
};