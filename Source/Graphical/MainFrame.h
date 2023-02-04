/*
  ==============================================================================

	MainFrame.h
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioFileFrame.h"
#include "SynthFrame.h"
#include "GrainsFrame.h"

class MainFrame : public juce::Component {
public:
	MainFrame();
	~MainFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainFrame)

		SynthFrame synthFrame;
	GrainsFrame grainsFrame;
	AudioFileFrame audioFileFrame;

};