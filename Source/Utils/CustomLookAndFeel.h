/*
  ==============================================================================

	CustomLookAndFeel.h
	Created: 4 Feb 2023 3:43:16pm
	Author:  user

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "../../Ressources/Font/FuturaMedium.h"
#include "ParamsID.h"
#include "../Utils/MyColours.h"


class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
	CustomLookAndFeel();
	~CustomLookAndFeel();


	// virtual method to redraw a slider for custom needs
	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
