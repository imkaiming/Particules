/*
  ==============================================================================

    GrainsFrame.h
    Created: 3 Feb 2023 11:44:06pm
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

class GrainsFrame : public juce::Component {
public:
	GrainsFrame();
	~GrainsFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsFrame)
};