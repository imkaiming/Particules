/*
  ==============================================================================

	SelectionOverlay.h
	Created: 21 Mar 2023 8:32:44pm
	Author:  user

  ==============================================================================
*/




#include <juce_gui_basics/juce_gui_basics.h>
#include "../utils/MyColours.h"
#pragma once



class SelectionOverlay : public juce::Component
{
public:
	SelectionOverlay();
	~SelectionOverlay() = default;

	void paint(juce::Graphics&) override;
	void setPosition(float);
	void setSelection(float);

private:

	float start;
	float width;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectionOverlay)
};