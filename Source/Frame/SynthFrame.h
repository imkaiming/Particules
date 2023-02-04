/*
  ==============================================================================

	SynthFrame.h
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

class SynthFrame : public juce::Component
{
public:
	SynthFrame();
	~SynthFrame();

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFrame)

};