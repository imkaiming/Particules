/*
  ==============================================================================

	AudioFileFrame.h
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Component/SpectrumComponent.h"
#include "../Component/AudioFileComponent.h"
#include "../Utils/StateSaver.h"



class AudioFileFrame : public juce::Component {
public:
	AudioFileFrame(juce::DrawableButton*, juce::DrawableButton*, juce::DrawableButton*);
	~AudioFileFrame() override;
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	AudioFileComponent audioFileComponent;
	SpectrumComponent spectrumComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)

};
