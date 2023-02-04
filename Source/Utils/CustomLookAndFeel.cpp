/*
  ==============================================================================

	CustomLookAndFeel.cpp
	Created: 4 Feb 2023 3:43:16pm
	Author:  user

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
	setColour(juce::Slider::thumbColourId, juce::Colours::red);

	juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(
		BinaryData::CascadiaMonoPLRegular_otf,
		BinaryData::CascadiaMonoPLRegular_otfSize
	);
	setDefaultSansSerifTypeface(typeface);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	float diameter = juce::jmin(width, height);
	float radius = diameter / 2;
	float centerX = x + width / 2;
	float centerY = y + height / 2;

	// offset the position to center the dial
	float rx = centerX - radius;
	float ry = centerY - radius;

	// the angle of the tick with the scaled position of the slider
	float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

	juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);
	g.setColour(juce::Colours::coral);
	g.fillEllipse(dialArea);

	// path is to draw custom shapes
	juce::Path dialTick;
	g.setColour(juce::Colours::darkolivegreen);
	dialTick.addRectangle(0, -radius, 5.0f, radius * 0.5);
	g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));

	g.setColour(juce::Colours::black);
	g.drawEllipse(rx, ry, diameter, diameter, 5.0f);
}


