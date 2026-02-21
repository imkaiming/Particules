/*
  ==============================================================================

	CustomLookAndFeel.cpp
	Created: 4 Feb 2023 3:43:16pm
	Author:  user

  ==============================================================================
*/

#include "CustomLookAndFeel.h"
#include "../../Ressources/Font/FuturaMedium.h"
#include "../Framework/ParamsID.h"
#include "../Utils/MyColours.h"

CustomLookAndFeel::CustomLookAndFeel()
{
	setColour(juce::Slider::thumbColourId, MyColours::red);
	//setColour(juce::Slider::textBoxHighlightColourId, MyColours::black);
	//setColour(juce::Slider::textBoxBackgroundColourId, MyColours::black);
	setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

	futuraTypeface = juce::Typeface::createSystemTypefaceFor(
		FuturaMedium::FuturaMedium_ttf,
		FuturaMedium::FuturaMedium_ttfSize
	);

	//LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(typeface);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}


void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{

	const float diameter = static_cast<float>(juce::jmin(width, height)) - 4.0f;
	const float radius = diameter / 2.0f;
	const float centerX = static_cast<float>(x + width / 2.0f);
	const float centerY = static_cast<float>(y + height / 2.0f);
	const float radiusX = centerX - radius;
	const float radiusY = centerY - radius;

	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	const float lineWidth = juce::jmin(15.0f, radius * 0.1f);
	const float arcRadius = radius - lineWidth * 0.5f;


	g.setColour(MyColours::black);
	g.fillEllipse(radiusX, radiusY, radius * 2.0f, radius * 2.0f);

	g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
	juce::Path outlineArc;
	outlineArc.addCentredArc(centerX, centerY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
	g.strokePath(outlineArc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	g.setColour(MyColours::jungleGreen);
	juce::Path valueArc;
	valueArc.addCentredArc(centerX, centerY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);
	g.strokePath(valueArc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	//setColour(juce::Slider::rotarySliderOutlineColourId, MyColours::cream);

}



