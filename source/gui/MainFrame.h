/*
  ==============================================================================

	MainFrame.h
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#pragma once


#include "../framework/Core.h"

#include "AudioFileFrame.h"
#include "SynthFrame.h"
#include "GrainsFrame.h"
#include "TitleFrame.h"
#include "StatusBarFrame.h"

struct UIContext;
class MainFrame : public juce::Component {
public:
	MainFrame(UIContext& uic);
	~MainFrame() = default;
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	SynthFrame synthFrame;
	GrainsFrame grainsFrame;
	AudioFileFrame audioFileFrame;
	StatusBarFrame statusBarFrame;
	TitleFrame titleFrame;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainFrame)
};