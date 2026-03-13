/*
  ==============================================================================

	CustomLookAndFeel.h
	Created: 4 Feb 2023 3:43:16pm
	Author:  user

  ==============================================================================
*/


#pragma once

//#include <juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.h>
#include "../framework/Core.h"



class CustomLookAndFeel: public juce::LookAndFeel_V4
{
public:
	CustomLookAndFeel();
	~CustomLookAndFeel();

	//void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
	//	float sliderPos, float minSliderPos, float maxSliderPos,
	//	const juce::Slider::SliderStyle style, juce::Slider& slider) override;

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
						  float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
	//void drawLabel(juce::Graphics& g, juce::Label& label) override;

	//juce::Font getTitleFont(juce::Label&) override
	//{
	//	return juce::Font("Arial", 16.0f, juce::Font::plain);
	//}

	//juce::Font getTextButtonFont(juce::TextButton&, int height) override
	//{
	//	return juce::Font("Verdana", height * 0.5f, juce::Font::bold);
	//}

	juce::Typeface::Ptr getFuturaTypeface() const
	{
		return futuraTypeface;
	}

private:

	juce::Typeface::Ptr futuraTypeface;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
