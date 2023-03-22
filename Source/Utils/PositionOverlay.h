
#include <JuceHeader.h>
#include "MyColours.h"
#pragma once



class PositionOverlay : public juce::Component
{
public:
	PositionOverlay();
	~PositionOverlay();

	void paint(juce::Graphics&) override;
	void setPosition(float);
	float getPosition();

private:

	float positionValue;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionOverlay)
};