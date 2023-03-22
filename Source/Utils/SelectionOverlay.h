/*
  ==============================================================================

	SelectionOverlay.h
	Created: 21 Mar 2023 8:32:44pm
	Author:  user

  ==============================================================================
*/




#include <JuceHeader.h>
#include "MyColours.h"
#pragma once



class SelectionOverlay : public juce::Component
{
public:
	SelectionOverlay();
	~SelectionOverlay();

	void paint(juce::Graphics&) override;
	void setPosition(float);
	void setSelection(float);

private:

	float positionValue;
	float selectionValue;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectionOverlay)
};