/*
  ==============================================================================

	SynthFrame.h
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../Utils/StateParameters.h"
#pragma once

class SynthFrame : public juce::Component
{
public:
	SynthFrame(ValueTreeState* apvts, StateParameters* stateParams);
	~SynthFrame();

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	ValueTreeState* apvts;
	StateParameters* stateParams;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFrame)

};