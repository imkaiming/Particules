/*
  ==============================================================================

	TitleFrame.h
	Created: 4 Feb 2023 1:27:14pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

class TitleFrame : public juce::Component {
public:
	TitleFrame();
	~TitleFrame();
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleFrame)
};