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



class AudioFileFrame : public juce::Component {
public:
	AudioFileFrame(AudioFileComponent*);
	~AudioFileFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	AudioFileComponent* audioFileComponent;
	SpectrumComponent spectrumComponent;
	//juce::TextButton openFileButton;
	//juce::ImageButton openFileButton;
	//std::unique_ptr<juce::Drawable> openFileSvgPtr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)

};
