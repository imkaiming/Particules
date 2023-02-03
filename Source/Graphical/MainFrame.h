/*
  ==============================================================================

	MainFrame.h
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MainFrame : public juce::Component {
public:
	MainFrame();
	~MainFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
};