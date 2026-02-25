
#include <juce_gui_basics/juce_gui_basics.h>
#include "../utils/MyColours.h"
#pragma once



class PositionOverlay : public juce::Component
{
public:
	PositionOverlay();
	~PositionOverlay() = default;

	void paint(juce::Graphics&) override;
	void setPosition(float);
	float getPosition();

private:

	float startPos;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionOverlay)
};