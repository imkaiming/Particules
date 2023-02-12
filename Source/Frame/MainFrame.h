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
#include "TitleFrame.h"
#include "StatusBarFrame.h"
#include "../Utils/StateSaver.h"

class MainFrame : public juce::Component {
public:
	MainFrame(juce::DrawableButton*, juce::DrawableButton*, juce::DrawableButton*);
	~MainFrame() override;
	//void init(StateSaver*);
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