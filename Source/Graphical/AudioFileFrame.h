/*
  ==============================================================================

	AudioFileFrame.h
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SpectrumComponent.h"

class AudioFileFrame : public juce::Component {
public:
	AudioFileFrame();
	~AudioFileFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)
		SpectrumComponent spectrumComponent;
		juce::TextButton openFileButton;

};