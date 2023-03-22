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
	setColour(juce::Slider::thumbColourId, MyColours::red);

	juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(
		FuturaMedium::FuturaMedium_ttf,
		FuturaMedium::FuturaMedium_ttfSize
	);

	//LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(typeface);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

/*
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
*/

/*
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)

{
	const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
	const float centreX = x + width * 0.5f;
	const float centreY = y + height * 0.5f;
	const float rx = centreX - radius;
	const float ry = centreY - radius;

	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	const float lineW = juce::jmin(15.0f, radius * 0.5f);
	const float arcRadius = radius - lineW * 0.5f;

	g.setColour(MyColours::black);
	g.fillEllipse(rx, ry, radius * 2.0f, radius * 2.0f);

	g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
	juce::Path valueArc;
	valueArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);
	g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
	juce::Path outlineArc;
	outlineArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
	g.strokePath(outlineArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
*/

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

	//setColour(juce::Slider::rotarySliderOutlineColourId, MyColours::cream);

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

}

/*
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos,
	const juce::Slider::SliderStyle style, juce::Slider& slider)
{

	g.fillAll(MyColours::black);


	g.setColour(MyColours::brightBlue);
	g.fillRect(x, y, (int)sliderPos - x, height);

	g.setColour(slider.findColour(juce::Slider::textBoxTextColourId).withMultipliedAlpha(0.5f));
	g.drawRect(x, y, (int)sliderPos - x, height);


}*/

/*
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos,
	const juce::Slider::SliderStyle style, juce::Slider& slider)
{

	g.setColour(MyColours::lavender);


	g.setColour(MyColours::black);
	g.fillRect(x, y, width, height);

	g.setColour(juce::Colours::white);
	g.fillRect(x, y, width, height);

	g.setColour(juce::Colours::black);
	g.drawRect(x, y, width, height);

	g.setColour(juce::Colours::black);
	g.setFont(15.0f);
	g.drawText(juce::String(slider.getValue()), x, y, width, height, juce::Justification::centred);
}*/


//void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
//{
//	//g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::centred, true);
//
//
//	// Call the base class to draw the label with the new font
//	LookAndFeel_V4::drawLabel(g, label);
//}